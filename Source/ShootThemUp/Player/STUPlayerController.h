// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponentComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()
    ASTUPlayerController();
public:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTURespawnComponentComponent* STURespawnComponentComponent;

    virtual void OnPossess(APawn *InPawn) override;
};
