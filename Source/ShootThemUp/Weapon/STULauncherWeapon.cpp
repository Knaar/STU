// Fill out your copyright notice in the Description page of Project Settings.


#include "STULauncherWeapon.h"

#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
    Super::StartFire();
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    Super::MakeShot();
    const FTransform MuzzleRotator(FRotator::ZeroRotator, GetMuzzleLocation());
    const auto SpawnedProjectile=UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),Projectile,MuzzleRotator);

    UGameplayStatics::FinishSpawningActor(SpawnedProjectile,MuzzleRotator);
}
