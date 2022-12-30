 // Fill out your copyright notice in the Description page of Project Settings.


#include "STULauncherWeapon.h"

#include "STUProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld()||IsAmmoEmpty())
    {
        StopFire();
        return;
    }
    DecreaseAmmo();
    SpawnMuzzleVFX();
    

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

    
    FHitResult HitResult;
    //Задаём нараметры коллизии
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    
    //Блок стрельбы лучом, для определения, куда полетит ракета
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    
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
        Projectile->SetOwner(GetOwner());
        Projectile->SetShootDirection(Direction);
        Projectile->FinishSpawning(MuzzleTransformLocation);
    }
    

    
}
/*
 AController * ASTULauncherWeapon::GetController() const
 {
    const auto Pawn=Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
 }
*/
 