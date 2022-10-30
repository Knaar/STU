// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API AGameHUD : public AHUD
{
    GENERATED_BODY()
    
    virtual void BeginPlay() override;
    
    virtual void DrawHUD() override;

    void DrawLineFunc();
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    TSubclassOf<UUserWidget>SelectedUserWidget;
    
};
