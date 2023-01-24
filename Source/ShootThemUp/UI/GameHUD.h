// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "GameFramework/HUD.h"
#include "ShootThemUp/STUCoreTypes.h"
#include "GameHUD.generated.h"


UCLASS()
class SHOOTTHEMUP_API AGameHUD : public AHUD
{
    GENERATED_BODY()
    
    virtual void BeginPlay() override;


public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    TSubclassOf<UUserWidget>PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;
private:
    UPROPERTY()
    TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;

    UPROPERTY()
    USTUBaseWidget* CurrentWidget = nullptr;
    //Callback Функция
    void OnMatchStateChanged(ESTUMatchState State);
};
