// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUPlayerState.h"
#include "UObject/Object.h"
#include "STUUtils.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUUtils : public UObject
{
    GENERATED_BODY()


};

class STUUtils
{
public:
    bool static AreEnemies(AController *Controller1, AController *Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2)
            return false;

        const auto PlayerState1= Cast<ASTUPlayerState>(Controller1->PlayerState);
        const auto PlayerState2= Cast<ASTUPlayerState>(Controller2->PlayerState);

        //если GetTeamId не совпадают-враги
        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamId() != PlayerState2->GetTeamId(); 
    }
};
