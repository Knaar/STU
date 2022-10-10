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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;

    UPROPERTY(VisibleAnywhere)
    TArray<ABaseWeapon*> Weapons;

    UPROPERTY()
    ABaseWeapon* CurrentWeapon=nullptr;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    FName EquippedWeaponSocket="EquippedWeaponSocket";

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    FName InventoryWeaponSocket="InventoryWeaponSocket";

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
    UAnimMontage* AnimEquip;

    int32 CurrentWeaponIndex=0;

    bool bChangeWeaponInProgress=false;

    void SpawnWeapon();
    void AttachWeaponToMesh(ABaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);
    void NextWeapon();
    
    void StartFire();
    void StopFire();
    
    void SwapWeaponAnimPlay(UAnimMontage* AnimMontage);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* SkeletalMesh);

    bool CanFire();
    bool CanChangeWeapon();
};




