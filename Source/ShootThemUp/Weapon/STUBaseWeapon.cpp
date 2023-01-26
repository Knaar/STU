#include "STUBaseWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"


DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false; 
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon  mesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());
    checkf(DefaultAmmo.Bullets>0, TEXT("Bullets count couldn't be less or equeal zero"));
    checkf(DefaultAmmo.Clips>0, TEXT("Clips count couldn't be less or equeal zero"));
    CurrentAmmo = DefaultAmmo;
}


FVector ASTUBaseWeapon::GetMuzzleLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::bCanReload()
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(BaseWeaponLog, Warning, TEXT("No Bullets"));
    }
    
    CurrentAmmo.Bullets--;
    LogAmmo();
    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnReloadEmptyClip.Broadcast(this);
    }
}

void ASTUBaseWeapon::ChangeClip()
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

bool ASTUBaseWeapon:: IsAmmoEmpty()
{
    return !CurrentAmmo.bInfiniteWeapon && IsClipEmpty() && CurrentAmmo.Clips == 0;
}

bool ASTUBaseWeapon::IsClipEmpty()
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString LogAmmo = "Ammo: Bullets:" + FString::FromInt(CurrentAmmo.Bullets) + "Clips: ";
    LogAmmo += CurrentAmmo.bInfiniteWeapon ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    //UE_LOG(BaseWeaponLog, Warning, TEXT("%s"), *LogAmmo);
}

bool ASTUBaseWeapon::IsAmmoFull()
{
    return CurrentAmmo.bInfiniteWeapon||CurrentAmmo.Clips == DefaultAmmo.Clips;

}

bool ASTUBaseWeapon::TryToAddAmmo(int32 AmmoToAdd)
{
    if (IsAmmoFull()||CurrentAmmo.bInfiniteWeapon||AmmoToAdd <= 0)
        return false;

    if (IsAmmoEmpty())
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

UNiagaraComponent * ASTUBaseWeapon::SpawnMuzzleVFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(
        MuzzleVFX,
        WeaponMesh,
        MuzzleSocketName,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true);
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
    const auto STUCharacter=Cast<ACharacter>(GetOwner());
    if(!STUCharacter)return false;

    if(STUCharacter->IsPlayerControlled())
    {
        const auto Controller = GetController();
        if(!Controller) return false;
        Controller->GetPlayerViewPoint(ViewLocation,ViewRotation);
    }
    else
    {
        ViewLocation=GetMuzzleLocation();
        ViewRotation=WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    return  true;
}

AController * ASTUBaseWeapon::GetController() const
{
    const auto Pawn=Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
