// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "STUAIController.generated.h"

class USTURespawnComponentComponent;
class USTUAIPerceptionComponent;
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASTUAIController();
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnPossess(APawn *InPawn) override;

    //Perception component
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTUAIPerceptionComponent* STUAIPerceptionComponent;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTURespawnComponentComponent* STURespawnComponentComponent;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
    FName FocusOnKeyName="EnemyActor";
private:
    AActor* GetFocusOnActor()const;
};
