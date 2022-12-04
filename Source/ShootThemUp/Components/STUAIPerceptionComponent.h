// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
public:
    //Создаём ф-ю, которая будет возвращать указатьель на актора, которого увидел бот
    AActor* GetClosestEnemy()const;
};
