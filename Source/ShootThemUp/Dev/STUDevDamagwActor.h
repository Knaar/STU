// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamagwActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamagwActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUDevDamagwActor();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
    float Radius = 300;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
    float DamageDealing = 10.0f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    FColor SphereColor=FColor::Red;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    bool bDoFullDamage=false;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
    TSubclassOf<UDamageType> DamageType;
};
