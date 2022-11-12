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

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Settings")
    USphereComponent* SphereComponent;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Settings")
    UStaticMeshComponent* StaticMesh;
    
    virtual void BeginPlay() override;	
    virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

    virtual bool IsGiftPickedUp(AActor *Actor);
    
    void PickupGift();
    void RestoreGift();

    //Карусель Пикапа

    float RotationValue=0;

    void MakeRandRotation();
};
    

