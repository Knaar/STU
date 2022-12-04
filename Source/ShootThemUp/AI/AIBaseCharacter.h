// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "AIBaseCharacter.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API AAIBaseCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
public:
    AAIBaseCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Behavior Tree")
    UBehaviorTree* BehaviorTree;
};
