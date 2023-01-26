#include "STURifleWeapon.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "STUWeaponFXComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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
    InitFX();
    GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ThisClass::MakeShot, FireRate, true,0.0f);
    
}

void ASTURifleWeapon::StopFire()
{
    Super::StopFire();
    SetVFXActive(false);
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

    const auto Controller=GetController();
    
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
        Target->TakeDamage(BulletDamage,FDamageEvent{},GetController(),this);
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

void ASTURifleWeapon::InitFX()
{
    if(!MuzzleVFXComponent)
    {
        MuzzleVFXComponent=SpawnMuzzleVFX();
    }
   
    if(!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound,WeaponMesh,MuzzleSocketName);
    }

    SetVFXActive(true);
}

void ASTURifleWeapon::SetVFXActive(bool IsActive)
{
    if(MuzzleVFXComponent)
    {
        MuzzleVFXComponent->SetPaused(!IsActive);
        MuzzleVFXComponent->SetVisibility(IsActive);
    }
    
    if(FireAudioComponent)
    {
        IsActive? FireAudioComponent->Play() : FireAudioComponent->Stop();
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
/*
AController * ASTURifleWeapon::GetController() const
{
    const auto Pawn=Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
*/