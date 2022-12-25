// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUAmmoPercentDecorator.generated.h"

class ASTUBaseWeapon;
class ABaseWeapon;
UCLASS()
class SHOOTTHEMUP_API USTUAmmoPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
    USTUAmmoPercentDecorator();

protected:
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
    float PercentToGift=0.3f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
    TSubclassOf<ASTUBaseWeapon> WeaponType;
    
};
