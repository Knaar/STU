#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere,Category="Weapon")
    USphereComponent* SphereCollision;
};
