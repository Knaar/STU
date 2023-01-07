// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootThemUp/STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class USTUPlayerStatRowWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    TSubclassOf <UUserWidget> PlayerStatRowWidgetClass;

private:
    void OnMatchStateChanged(ESTUMatchState State);
    void UpdatePlayerStat();
};
