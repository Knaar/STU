// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpGift.h"
#include "HealthPickUpGift.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API AHealthPickUpGift : public ABasePickUpGift
{
	GENERATED_BODY()
    
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="HeathSettings",meta=(ClampMin=0,ClampMax=200))
    float SetHealthAmount=100.0f;
    
    virtual bool IsGiftPickedUp(AActor *Actor) override;
};
