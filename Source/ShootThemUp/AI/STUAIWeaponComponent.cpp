// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIWeaponComponent.h"

#include "STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    if (CurrentWeapon->IsNoAmmo())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire(); // This equal Super::StartFire()?
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    Super::NextWeapon();
}
