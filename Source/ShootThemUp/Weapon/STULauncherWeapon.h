// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:   
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    USoundCue* NoAmmoSound;
    
    virtual void StartFire() override;

    virtual void MakeShot() override;

    //AController* GetController() const;
};
