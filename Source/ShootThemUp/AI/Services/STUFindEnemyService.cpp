// Fill out your copyright notice in the Description page of Project Settings.


#include "STUFindEnemyService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShootThemUp/Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName="FindEnemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto BlackBoard=OwnerComp.GetBlackboardComponent();
    if(BlackBoard)
    {
        const auto Controller=OwnerComp.GetAIOwner();
        const auto Component=Controller->GetComponentByClass(USTUAIPerceptionComponent::StaticClass());
        const auto PerceptionComponent=Cast<USTUAIPerceptionComponent>(Component);
        if(PerceptionComponent)
        {
            BlackBoard->SetValueAsObject(EnemyActorKey.SelectedKeyName,PerceptionComponent->GetClosestEnemy());
            //UE_LOG(LogTemp,Warning,TEXT("Actor Found"));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
}
