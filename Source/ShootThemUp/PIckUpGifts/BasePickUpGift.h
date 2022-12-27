#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickUpGift.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class SHOOTTHEMUP_API ABasePickUpGift : public AActor
{
    GENERATED_BODY()
	
public:	
	
    ABasePickUpGift();

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PickUp")
    USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PickUp")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PickUp")
    float RespawnTime = 5.0f;
    
    virtual void BeginPlay() override;	
    virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

    virtual bool GivePickUpTo(APawn *Pawn);

    FTimerHandle TimerToRestore;
        
    void PickupWasTaken();//PickupGift();
    void Respawn();//RestoreGift();

    //Карусель Пикапа

    float RotationValue=0;

    void MakeRandRotation();

    bool CouldBeTaken() const;
};
    

