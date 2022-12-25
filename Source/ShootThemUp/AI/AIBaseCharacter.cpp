// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseCharacter.h"
#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIBaseCharacter::AAIBaseCharacter(const FObjectInitializer& ObjInit)
    :Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComp"))
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

void AAIBaseCharacter::OnPlayerDeath()
{
    Super::OnPlayerDeath();
    const auto STUController=Cast<AAIController>(Controller);
    if(STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}

