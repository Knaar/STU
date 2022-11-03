#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnReloadEmptyClip,ABaseWeapon*)

USTRUCT(BlueprintType)
struct FMyWeaponImageData
{
    GENERATED_BODY()
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    UTexture2D* WeaponImage;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    UTexture2D* AimImage;
    
};

USTRUCT(BlueprintType)
struct FMyAmmo
{
    GENERATED_USTRUCT_BODY()
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Ammo")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Ammo",meta=(EditCondition="!bInfiniteWeapon"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Ammo")
    bool bInfiniteWeapon;
    
};

UCLASS()
class SHOOTTHEMUP_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();
    virtual void BeginPlay() override;
    
    virtual void StartFire();
    virtual void StopFire();

    virtual void MakeShot();   

    FOnReloadEmptyClip OnReloadEmptyClip;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="My Settings")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    FName SocketName="MuzzleSocket";

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float TraceLenght=5000.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float BulletDamage=10.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float FireRate=0.01f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    float FireAccuracy=1.5f;
    
    FTimerHandle ShootTimer;

    APlayerController* GetPlayerController()const;

    FVector GetMuzzleLocation()const;

    //Патрики
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="My Settings")
    FMyAmmo DefaultAmmo{15,10,false};

    FMyAmmo CurrentAmmo;

    bool bCanReload();
    
    void DecreaseBullets();
    void ChangeClip();
    bool IsNoAmmo();
    bool IsClipEmpty();
    void LogAmmo();

    //UI
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="UI")
    FMyWeaponImageData MyWeaponImageData;
    
    FMyWeaponImageData GetMyWeaponImageData() const    {return MyWeaponImageData;}
    
    FMyAmmo GetMyWeaponAmmoData() const    {return CurrentAmmo;}

    //Gift Add Ammo
    bool IsAmmoFull();
    bool TryToAddAmmo(int32 AmmoToAdd);
};
