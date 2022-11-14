#include "BaseWeapon.h"
#include "GameFramework/Character.h"


DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon  mesh");
    SetRootComponent(WeaponMesh);
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(GetWorld());
    checkf(DefaultAmmo.Bullets>0, TEXT("Bullets count couldn't be less or equeal zero"));
    checkf(DefaultAmmo.Clips>0, TEXT("Clips count couldn't be less or equeal zero"));

    CurrentAmmo = DefaultAmmo;
}

void ABaseWeapon::StartFire()
{
}

void ABaseWeapon::StopFire()
{
}

void ABaseWeapon::MakeShot()
{
}

APlayerController *ABaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller)
        return nullptr;
    return Controller;
}

FVector ABaseWeapon::GetMuzzleLocation() const
{
    return WeaponMesh->GetSocketLocation(SocketName);
}

bool ABaseWeapon::bCanReload()
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ABaseWeapon::DecreaseBullets()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(BaseWeaponLog, Warning, TEXT("No Bullets"));
    }
    CurrentAmmo.Bullets--;
    LogAmmo();
    if (IsClipEmpty() && !IsNoAmmo())
    {
        StopFire();
        OnReloadEmptyClip.Broadcast(this);
    }
}

void ABaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.bInfiniteWeapon)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(BaseWeaponLog, Warning, TEXT("No Ammo"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    UE_LOG(BaseWeaponLog, Warning, TEXT("Reloading"));
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ABaseWeapon::IsNoAmmo()
{
    return !CurrentAmmo.bInfiniteWeapon && IsClipEmpty() && CurrentAmmo.Clips == 0;
}

bool ABaseWeapon::IsClipEmpty()
{
    return CurrentAmmo.Bullets == 0;
}

void ABaseWeapon::LogAmmo()
{
    FString LogAmmo = "Ammo: Bullets:" + FString::FromInt(CurrentAmmo.Bullets) + "Clips: ";
    LogAmmo += CurrentAmmo.bInfiniteWeapon ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    //UE_LOG(BaseWeaponLog, Warning, TEXT("%s"), *LogAmmo);
}

bool ABaseWeapon::IsAmmoFull()
{
    return CurrentAmmo.bInfiniteWeapon||CurrentAmmo.Clips == DefaultAmmo.Clips;

}

bool ABaseWeapon::TryToAddAmmo(int32 AmmoToAdd)
{
    if (IsAmmoFull()||CurrentAmmo.bInfiniteWeapon||AmmoToAdd <= 0)
        return false;

    if (IsNoAmmo())
    {
        CurrentAmmo.Clips = FMath::Clamp(AmmoToAdd, 0, DefaultAmmo.Clips+1);
        OnReloadEmptyClip.Broadcast(this);
       
    }
    else if(CurrentAmmo.Clips<DefaultAmmo.Clips)
    {
        const auto NextClipsAmount=CurrentAmmo.Clips+AmmoToAdd;
        if(DefaultAmmo.Clips-NextClipsAmount>=0)
        {
            CurrentAmmo.Clips=NextClipsAmount;
        }
        else
        {
            CurrentAmmo.Clips=DefaultAmmo.Clips;
            CurrentAmmo.Bullets=DefaultAmmo.Bullets;
        }
    }
    else
    {
        CurrentAmmo.Bullets=DefaultAmmo.Bullets;
    }

    return true;
}
