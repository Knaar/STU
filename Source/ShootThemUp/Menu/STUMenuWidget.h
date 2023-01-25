// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootThemUp/UI/STUBaseWidget.h"
#include "ShootThemUp/UI/STULevelItemWidget.h"
#include "STUMenuWidget.generated.h"

class USoundCue;
class USTUGameInstance;
class UButton;
class UHorizontalBox;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()
    
protected:

    UPROPERTY(meta=(BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta=(BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    USoundCue* StartGameSound;

    UPROPERTY(meta=(BindWidgetAnim),Transient)
    UWidgetAnimation* LoadGameAnimation;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;
    
    virtual void NativeOnInitialized() override;

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation *Animation) override;
private:
    UPROPERTY()
    TArray<USTULevelItemWidget*> LevelItemWidgets;
    
    UFUNCTION()
    void OnStartGame();
    
    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
    USTUGameInstance* GetSTUGameInstance() const;
};
