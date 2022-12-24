// Fill out your copyright notice in the Description page of Project Settings.


#include "STUFireService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShootThemUp/AI/STUAIWeaponComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"

USTUFireService::USTUFireService()
{
    NodeName="Shoot";
}

void USTUFireService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Controller=OwnerComp.GetAIOwner();
    const auto BlackBoard=OwnerComp.GetBlackboardComponent();

    const auto Pawn=Controller->GetCharacter();
    if (Pawn)
    {
        UE_LOG(LogTemp,Warning,TEXT("Pawn valid"));
    }
     else
     {
         UE_LOG(LogTemp,Warning,TEXT("Pawn not valid"));
     }
    const auto HasAim=BlackBoard && BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if(Controller)
    {
        const auto Component=Pawn->GetComponentByClass(UWeaponComponent::StaticClass());
        const auto WeaponComponent=Cast<UWeaponComponent>(Component);
        UE_LOG(LogTemp,Warning,TEXT("Searching WeaponComponent"));
        if(WeaponComponent)
        {
            HasAim? WeaponComponent->StartFire():WeaponComponent->StopFire();
            UE_LOG(LogTemp,Warning,TEXT("Try to shoot"));
        }
        else
        {
            UE_LOG(LogTemp,Warning,TEXT("WeaponComponentNotFound"));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
