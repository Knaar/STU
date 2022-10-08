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
    
    virtual void Fire();

    void MakeShot();   

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="My Settings")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    FName SocketName="MuzzleSocket";

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float TraceLenght=5000.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float BulletDamage=10.0f;
};
