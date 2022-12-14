// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    //BondWidget ассоциирует кнопку в С++ с кнопкой в Blueprint
    UPROPERTY(meta=(BindWidget))
    UButton* ClearPauseButton;

private:
    //Callback функция, которая вызывается по нажатию кнопки ClearPauseButton
    UFUNCTION()
    void OnClearPause();
};
