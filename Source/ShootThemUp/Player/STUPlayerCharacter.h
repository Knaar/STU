// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    USpringArmComponent *SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Category")
    USphereComponent* CameraCollisionComponent;

    virtual void BeginPlay() override;
    virtual void OnPlayerDeath() override;
public:
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    bool bWantsToRun = false;
    void OnStartRunning();
    void OnStopRunning();
private:
    UFUNCTION()
    void OnCameraCollisionBeginOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    void CheckCameraOverlap();

};
