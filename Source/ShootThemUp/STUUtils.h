// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUPlayerState.h"
#include "UObject/Object.h"




class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        // Get Player health component:
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
    
    bool static AreEnemies(AController *Controller1, AController *Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2)
            return false;

        const auto PlayerState1= Cast<ASTUPlayerState>(Controller1->PlayerState);
        const auto PlayerState2= Cast<ASTUPlayerState>(Controller2->PlayerState);

        //если GetTeamId не совпадают-враги
        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamId() != PlayerState2->GetTeamId(); 
    }

    static  FText TextFromInt(int32 Number)
    {
        return FText::FromString(FString::FromInt(Number));
    }
};
