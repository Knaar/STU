#include "STURifleWeapon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "STUWeaponFXComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(RifleWeaponLog, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent=CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    Super::StartFire();
    InitMuzzleComponent();
    GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ThisClass::MakeShot, FireRate, true,0.0f);
    
}

void ASTURifleWeapon::StopFire()
{
    Super::StopFire();
    SetVFXVisibility(false);
    GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
}



void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld()||IsAmmoEmpty())
    {
        StopFire();
        return;
    }
    DecreaseAmmo();
    

    //Узнаём напрвление камеры для определения траектории стрельбы
    FVector CameraLocation;
    FRotator CameraRotation;

    const auto STUCharacter=Cast<ACharacter>(GetOwner());
    if(!STUCharacter) return;

    const auto Controller=GetPlayerController();
    
    if(STUCharacter->IsPlayerControlled())
    {
        
        if(!Controller) return;
        Controller->GetPlayerViewPoint(CameraLocation,CameraRotation);
    }
    else
    {
        CameraLocation=GetMuzzleLocation();
        CameraRotation=WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    

    //Создаём радианы для разброса пуль
    const auto HalfRad=FMath::DegreesToRadians(FireAccuracy);
    
    //Задаем переменные Положения соккета,Начальной точки, конечной, и направления
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector StartTrace = CameraLocation;//SocketTransform.GetLocation();
    const FVector TraceDirection = FMath::VRandCone(CameraRotation.Vector(),HalfRad);//SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = StartTrace + TraceDirection * TraceLenght;

    //Задаём нараметры коллизии
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial=true;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);

    FVector EndTraceVfx=TraceEnd;
    if (HitResult.bBlockingHit)
    {
        WeaponFXComponent->PlayUNiagaraSystemReleased(HitResult);

        EndTraceVfx=HitResult.ImpactPoint;
        
        //DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 0.2f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(),HitResult.Location,10,10,FColor::Orange,false,0.5f,0,3.0f);
        //UE_LOG(RifleWeaponLog, Warning, TEXT("Bone: %s"),*HitResult.BoneName.ToString());
        
        const auto Target= HitResult.GetActor();
        if(!Target)return;
        Target->TakeDamage(BulletDamage,FDamageEvent{},Controller,this);
    }

    SpawnTraceVFX(SocketTransform.GetLocation(),EndTraceVfx);
    
    /*
    else
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 0.5f, 0, 3.0f);
    }
    Super::MakeShot();
    */
}

void ASTURifleWeapon::InitMuzzleComponent()
{
    if(!MuzzleVFXComponent)
    {
        MuzzleVFXComponent=SpawnMuzzleVFX();
    }
    SetVFXVisibility(true);
}

void ASTURifleWeapon::SetVFXVisibility(bool Visibility)
{
    if(MuzzleVFXComponent)
    {
        MuzzleVFXComponent->SetPaused(!Visibility);
        MuzzleVFXComponent->SetVisibility(Visibility);
    }
}

void ASTURifleWeapon::SpawnTraceVFX(const FVector &StartTrace, const FVector &EndTrace)
{
    const auto TraceVFXComponent=UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),TraceVFX,StartTrace);
    if(TraceVFXComponent)
    {
        TraceVFXComponent->SetNiagaraVariableVec3(TraceTargetName,EndTrace);
    }
}
