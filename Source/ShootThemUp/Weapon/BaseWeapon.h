#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
UCLASS()
class SHOOTTHEMUP_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SkeletalMesh")
    USkeletalMeshComponent* SkeletalMesh;
};
