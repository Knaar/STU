// Fill out your copyright notice in the Description page of Project Settings.


#include "STUChangeWeaponService.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "ShootThemUp/Components/WeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName="ChangeWeapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Controller= OwnerComp.GetAIOwner();
    const auto BlackBoard= OwnerComp.GetBlackboardComponent();
    if(!Controller||!BlackBoard)return;
    
    const auto Pawn= Controller->GetCharacter();
    if(!Pawn)return;

    const auto Component=Pawn->GetComponentByClass(UWeaponComponent::StaticClass());
    if(!Component)return;

    const auto WeaponComponent=Cast<UWeaponComponent>(Component);
    if(!WeaponComponent)return;

    float FirstNumber = 0.0f;
    float RandomNumber = FMath::FRandRange(0.0f,100.0f);
    if(FMath::IsNearlyEqual(FirstNumber,RandomNumber,Probability*10))
    {
        WeaponComponent->NextWeapon();
    }
    
}
