// Fill out your copyright notice in the Description page of Project Settings.


#include "STUHealthPercentDecorator.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "ShootThemUp/Components/STUHeathComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "HealthPercent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(!Controller)return false;

    const auto Pawn=Controller->GetCharacter();
    if(!Controller)return false;

    const auto Component=Pawn->GetComponentByClass(USTUHeathComponent::StaticClass());
    if(!Pawn)return false;

    const auto HealthComponent=Cast<USTUHeathComponent>(Component);
    if(!HealthComponent||HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent()<MinHealthPercentToGift;
    
}
