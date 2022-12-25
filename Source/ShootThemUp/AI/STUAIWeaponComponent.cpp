// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIWeaponComponent.h"

#include "STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    if (CurrentWeapon->IsAmmoEmpty())
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
    if(!CanFire())return;

    int32 NextIndex=(CurrentWeaponIndex++)%Weapons.Num();
    while (NextIndex!=CurrentWeaponIndex)
    {
        if(!Weapons[NextIndex]->IsAmmoEmpty())break;
        NextIndex=(NextIndex++)%Weapons.Num();
    }
    Super::NextWeapon();

    if(CurrentWeaponIndex!=NextIndex)
    {
        CurrentWeaponIndex=NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
