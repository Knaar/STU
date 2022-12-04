// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIController.h"
#include "AIBaseCharacter.h"
#include "ShootThemUp/Components/STUAIPerceptionComponent.h"


ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent=CreateDefaultSubobject<USTUAIPerceptionComponent>("PerceptionComponent");
    
    //Нам нужно вызвать ф-я Ai контроллера, которая называется
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const auto AimActor=STUAIPerceptionComponent->GetClosestEnemy();
    SetFocus(AimActor);
}

void ASTUAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter= Cast<AAIBaseCharacter>(InPawn);
    
    if(STUCharacter)
    {
        RunBehaviorTree(STUCharacter->BehaviorTree);
    }
    
}
