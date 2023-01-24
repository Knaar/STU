// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "AIBaseCharacter.generated.h"

class USTUHealthBarWidget;
class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class SHOOTTHEMUP_API AAIBaseCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
public:
    AAIBaseCharacter(const FObjectInitializer& ObjInit);
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Behavior Tree")
    UBehaviorTree* BehaviorTree;

protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    float HealthVisibilityDistance = 5000.f;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Behavior Tree")
    UWidgetComponent* HealthWidgetComponent;
    
public:
    virtual void Tick(float DeltaSeconds) override;
protected:
    virtual void BeginPlay() override;
    virtual void OnPlayerDeath() override;

    virtual void OnHealthChange(float Health, float HealthDelta) override;

private:
    void UpdateHealthWidgetVisibility();
};
