// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    void SetTeamID(int32 ID) { TeamID = ID;}
    int32 GetTeamId() const {return TeamID;}

    void SetTeamColor(const FLinearColor& Color){TeamColor=Color;}
    FLinearColor GetTeamColor()const {return TeamColor; }

private:
    //Номер команды
    int32 TeamID;

    //Цвет команды
    FLinearColor TeamColor;

public:

    void AddKill() { ++KillsNum;}

    UFUNCTION(BlueprintCallable)
    int32 GetKillsNum() const {return KillsNum;}
    
    void AddDeath(){++DeathsNum;}

    UFUNCTION(BlueprintCallable)
    int32 GetDeathsNum() const {return DeathsNum;}

    void LogInfo();

private:
    //Подсчёт количества убийств
    int32 KillsNum = 0;

    //Подсчёт количества смертей
    int32 DeathsNum = 0;
    
};
