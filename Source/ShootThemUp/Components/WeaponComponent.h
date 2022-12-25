#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "WeaponComponent.generated.h"

struct FMyWeaponImageData;
struct FMyAmmo;
class ABaseWeapon;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FMyWeaponData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABaseWeapon> DataWeapon;

    UPROPERTY(EditAnywhere)
    UAnimMontage *DataAnimMontage;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWeaponComponent();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TArray<FMyWeaponData> WeaponData;

    UPROPERTY()
    ABaseWeapon *CurrentWeapon = nullptr;
    
    UPROPERTY(VisibleAnywhere)
    TArray<ABaseWeapon*> Weapons;
    
    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage *CurrentAnimEquipMontage=nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage *CurrentReloadAnimMontage = nullptr;
    
    UPROPERTY(VisibleDefaultsOnly)
    FName EquippedWeaponSocket = "EquippedWeaponSocket";

    UPROPERTY(VisibleDefaultsOnly)
    FName InventoryWeaponSocket = "InventoryWeaponSocket";

    int32 CurrentWeaponIndex = 0;
    bool bChangeWeaponInProgress = false;
    bool bReloadingInProgress=false;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    void SpawnWeapon();
    void AttachWeaponToMesh(ABaseWeapon *Weapon, USceneComponent *SceneComponent, const FName &SocketName);
    void EquipWeapon(int32 WeaponIndex);
    virtual void NextWeapon();

    virtual void StartFire();
    void StopFire();

    void PlayAnimation(UAnimMontage *AnimMontage);
    void InitAnimations();
    
    //Call Back Function
    void OnEquipFinished(USkeletalMeshComponent *SkeletalMesh);
    void OnReloadFinished(USkeletalMeshComponent *SkeletalMesh);

    bool CanFire();
    bool CanChangeWeapon();
    bool CanReload();

    void ObReloadEmptyClip(ABaseWeapon* AmmoEmptyWeapon);
    void Reload();

    void ChangeClip();

    //UI
    bool GetMyWeaponImageData(FMyWeaponImageData& ImgData) const;

    bool GetMyWeaponAmmo(FMyAmmo& AmmoData)const;

    //Add Bullets
    
    bool TryToAddBullets(int32 AddedAmmo, TSubclassOf<ABaseWeapon>Weapon);

    bool IsAmmoFull();
};



