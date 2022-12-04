// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ATestActor : public AActor
{
    GENERATED_BODY()

public:
    ATestActor();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Mesh")
    UStaticMeshComponent* BodyMesh;

};
