// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseCharacter.h"

#include "STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIBaseCharacter::AAIBaseCharacter(const FObjectInitializer& ObjInit)//:Super(ObjInit)
{
    AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass=ASTUAIController::StaticClass();

    bUseControllerRotationYaw=false;
    if(GetCharacterMovement())
    {
        GetCharacterMovement()->RotationRate=FRotator(0.0f,250.0f,0.0f);
        GetCharacterMovement()->bUseControllerDesiredRotation=true;
    }
}
