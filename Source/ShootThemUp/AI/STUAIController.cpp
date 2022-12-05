// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIController.h"
#include "AIBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
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
    const auto AimActor=GetFocusOnActor();
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

AActor * ASTUAIController::GetFocusOnActor() const
{
    if(!GetBlackboardComponent())return nullptr;
    const auto Actor=Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
    //UE_LOG(LogTemp,Warning,TEXT("Actor Found"));
    return Actor;
    
}
