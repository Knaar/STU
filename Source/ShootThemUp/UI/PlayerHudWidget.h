// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootThemUp/Weapon/BaseWeapon.h"
#include "PlayerHudWidget.generated.h"


UCLASS()
class SHOOTTHEMUP_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable,Category="Ui")
    float GetHealth();

    UFUNCTION(BlueprintCallable,Category="Ui")
    bool GetWeaponImage(FMyWeaponImageData& ImgData);
};
