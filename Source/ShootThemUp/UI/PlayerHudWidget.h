// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootThemUp/Weapon/STUBaseWeapon.h"
#include "PlayerHudWidget.generated.h"

class UWeaponComponent;
class USTUHeathComponent;

UCLASS()
class SHOOTTHEMUP_API UPlayerHudWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Ui")
    float GetHealth();

    UFUNCTION(BlueprintCallable, Category="Ui")
    bool GetWeaponImage(FMyWeaponImageData &ImgData);

    UFUNCTION(BlueprintCallable, Category="Ui")
    bool GetWeaponAmmoData(FMyAmmo &AmmoData);

    UWeaponComponent *GetWeaponComponent() const;

    USTUHeathComponent *GetHealthComponent() const;

    UFUNCTION(BlueprintCallable, Category="Ui")
    bool IsPlayerAlive();

    UFUNCTION(BlueprintCallable, Category="Ui")
    bool IsPlayerSpectator();

    //Рамка получения урона
    
    virtual bool Initialize() override;

    void OnNewPawn(APawn* NewPawn);

    void OnHealthChanged(float Health,float DeltaHealth);


    UFUNCTION(BlueprintImplementableEvent, Category="UI")
    void OnTakeDamage();
};
