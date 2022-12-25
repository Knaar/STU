// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STUWeaponFXComponent.h"
#include "STURifleWeapon.generated.h"

class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    
    GENERATED_BODY()
    ASTURifleWeapon();

public:
    virtual void BeginPlay() override;
    virtual void StartFire() override;
    virtual void StopFire() override;

   
    UFUNCTION()
    virtual void MakeShot() override;

    UPROPERTY(EditDefaultsOnly, Category="Settings")
    USTUWeaponFXComponent *WeaponFXComponent;
    
    //Muzzle VFX
    UPROPERTY()
    UNiagaraComponent* MuzzleVFXComponent;

    void InitMuzzleComponent();

    void SetVFXVisibility(bool Visibility);

    //Trace VFX

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="TraceVFX")
    UNiagaraSystem* TraceVFX;

    UPROPERTY(VisibleDefaultsOnly,Category="TraceVFX")
    FString TraceTargetName="TraceTarget";

    void SpawnTraceVFX(const FVector& StartTrace, const FVector& EndTrace);
};
