#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();
    
    virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, Category="Weapon")
    USphereComponent *SphereCollision;

    UPROPERTY(VisibleAnywhere, Category="Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
    float ProjectileLifeTime=5.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
    float BlowSphereRadius=200.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
    float BlowSphereDamage=100.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Weapon")
    bool bDoFullDamage=false;
    
    FVector ProjectileDirection;

    void SetShootDirection(const FVector &Direction){ ProjectileDirection = Direction; }

    UFUNCTION()
    void ComponentHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    AController *GetController();
};
