// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

class USTUHeathComponent;
class UTextRenderComponent;
class ABaseWeapon;
class UWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(BlueprintAssignable,Category="Settings")
    FOnButtonPressed OnButtonPressed;
private:
    UPROPERTY(EditDefaultsOnly, Category="Material")
    FName MaterialColorName = "Paint Color";
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UWeaponComponent* WeaponComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    USTUHeathComponent *HealthComponent;
    
    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage *AnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamageVelocity = FVector2D(900, 1200);

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);
    
    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
protected: 
    virtual void OnPlayerDeath();
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    UFUNCTION()
    void OnGroundLanded(const FHitResult &Hit);
public:
    void SetPlayerColor(const FLinearColor& Color);
};
