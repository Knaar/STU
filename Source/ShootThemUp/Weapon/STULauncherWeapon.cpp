// Fill out your copyright notice in the Description page of Project Settings.


#include "STULauncherWeapon.h"

#include "STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if(IsNoAmmo())return;
    SpawnMuzzleVFX();
    DecreaseBullets();
    
    const auto Controller=GetPlayerController();

    //Узнаём напрвление камеры для определения траектории стрельбы
    FVector CameraLocation;
    FRotator CameraRotation;
    Controller->GetPlayerViewPoint(CameraLocation,CameraRotation);

    FHitResult HitResult;
    //Задаём нараметры коллизии
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    
    //Блок стрельбы лучом, для определения, куда полетит ракета
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(SocketName);
    
    const FVector StartTrace = CameraLocation;//SocketTransform.GetLocation();
    const FVector TraceDirection =CameraRotation.Vector();
    const FVector TraceEnd = StartTrace + TraceDirection * TraceLenght;

    GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
    
    const FVector EndPoint=HitResult.bBlockingHit?HitResult.ImpactPoint:TraceEnd;
    const FVector Direction=(EndPoint-GetMuzzleLocation()).GetSafeNormal();
     
    const FTransform MuzzleTransformLocation(FRotator::ZeroRotator, GetMuzzleLocation());
    
    ASTUProjectile* Projectile=GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass,MuzzleTransformLocation);
    if (Projectile)
    {
        Projectile->SetShootDirection(Direction);
        Projectile->FinishSpawning(MuzzleTransformLocation);
    }
    

    
}
