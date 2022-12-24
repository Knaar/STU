// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseCharacter.h"

#include "STUAIController.h"
#include "STUAIWeaponComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

AAIBaseCharacter::AAIBaseCharacter(const FObjectInitializer& ObjInit)//:Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComp"))
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

void AAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    if(WeaponComponent)
    {
        UE_LOG(LogTemp,Warning,TEXT("I hawe Weapon Component"));
    }
    if(HealthComponent)
    {
        UE_LOG(LogTemp,Warning,TEXT("I have Health Component"));
    }
}
