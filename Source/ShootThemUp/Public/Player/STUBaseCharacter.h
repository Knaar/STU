// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHeathComponent;
class UTextRenderComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    // Sets default values for this character's properties
    
    ASTUBaseCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    USpringArmComponent *SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    USTUHeathComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    UTextRenderComponent *TextRenderComponent;

    UPROPERTY(EditDefaultsOnly,Category="Animation")
    UAnimMontage *AnimMontage;


    
    void OnHealthChanged(float Health);
    void OnPlayerDeath();

    
    // Передвижение персонажа
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    //Sprint

    bool bWantsToRun=false;
    void OnStartRunning();
    void OnStopRuning();


    // Обзор мышью
    /*
    void LookUp(float Amount);
    void TurnAround(float Amount);*/

    //Падение
    UPROPERTY(EditDefaultsOnly,Category="Movement")
    FVector2D VectorDamageVelocity=FVector2D(900,1200);

    UPROPERTY(EditDefaultsOnly,Category="Movement")
    FVector2D VectorDamage=FVector2D(10.0f,100.0f);
    
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    
    
};



