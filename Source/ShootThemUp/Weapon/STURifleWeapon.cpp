#include "STURifleWeapon.h"
#include "STUWeaponFXComponent.h"

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
    GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ThisClass::MakeShot, FireRate, true,0.0f);
    
}

void ASTURifleWeapon::StopFire()
{
    Super::StopFire();
    GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld()||IsNoAmmo())
    {
        StopFire();
        return;
    }
    DecreaseBullets();
    const auto Controller=GetPlayerController();

    //Узнаём напрвление камеры для определения траектории стрельбы
    FVector CameraLocation;
    FRotator CameraRotation;
    Controller->GetPlayerViewPoint(CameraLocation,CameraRotation);

    //Создаём радианы для разброса пуль
    const auto HalfRad=FMath::DegreesToRadians(FireAccuracy);
    
    //Задаем переменные Положения соккета,Начальной точки, конечной, и направления
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(SocketName);
    const FVector StartTrace = CameraLocation;//SocketTransform.GetLocation();
    const FVector TraceDirection = FMath::VRandCone(CameraRotation.Vector(),HalfRad);//SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = StartTrace + TraceDirection * TraceLenght;

    //Задаём нараметры коллизии
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial=true;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);

    if (HitResult.bBlockingHit)
    {
        WeaponFXComponent->PlayUNiagaraSystemReleased(HitResult);
       
        
        //DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 0.2f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(),HitResult.Location,10,10,FColor::Orange,false,0.5f,0,3.0f);
        //UE_LOG(RifleWeaponLog, Warning, TEXT("Bone: %s"),*HitResult.BoneName.ToString());
        
        const auto Target= HitResult.GetActor();
        if(!Target)return;
        Target->TakeDamage(BulletDamage,FDamageEvent{},Controller,this);
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 0.5f, 0, 3.0f);
    }Super::MakeShot();
}
