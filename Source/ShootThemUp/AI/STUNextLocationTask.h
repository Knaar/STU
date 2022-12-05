// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUNextLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUNextLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
    USTUNextLocationTask();

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    float Radius=1000.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    FBlackboardKeySelector AimLocationKey;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    bool SelfCenter=true;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(EditCondition="!SelfCenter"))
    FBlackboardKeySelector CenterActorKey;
    
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};
