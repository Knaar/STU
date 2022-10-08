#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"
class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    TSubclassOf<ABaseWeapon> WeaponClass;

    UPROPERTY()
    ABaseWeapon* CurrentWeapon=nullptr;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    FName SocketName="WeaponSocket";

    void AttachWeapon();
    void StartFire();
    void StopFire();
    
};
