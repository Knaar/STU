// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAmmoPercentDecorator.h"

#include "AIController.h"
#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "ShootThemUp/Components/WeaponComponent.h"

USTUAmmoPercentDecorator::USTUAmmoPercentDecorator()
{
    NodeName="AmmoPercent";
}

bool USTUAmmoPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(!Controller)return false;

    const auto Pawn=Controller->GetCharacter();
    if(!Controller)return false;

    const auto Component=Pawn->GetComponentByClass(UWeaponComponent::StaticClass());
    if(!Pawn)return false;

    const auto WeaponComponent=Cast<UWeaponComponent>(Component);
    if(!WeaponComponent) return false;

    float Determinator=WeaponComponent->CurrentWeapon->CurrentAmmo.Clips;
    float Determinator2=WeaponComponent->CurrentWeapon->DefaultAmmo.Clips;
 
    return WeaponComponent->NeedAmmo(WeaponType); //&&(Determinator/Determinator2)<PercentToGift;
   
}
