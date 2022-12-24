// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ShootThemUp/Components/WeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public UWeaponComponent
{
	GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void NextWeapon() override;
};
