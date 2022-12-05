// Fill out your copyright notice in the Description page of Project Settings.


#include "STUNextLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !BlackBoard)
        return EBTNodeResult::Failed;

    const auto Player = Controller->GetPawn();
    if (!Player)
        return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Player);
    if (!NavSys)
        return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    auto Location = Player->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(BlackBoard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor)
            return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }

    const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!Found)
        return EBTNodeResult::Failed;

    BlackBoard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
