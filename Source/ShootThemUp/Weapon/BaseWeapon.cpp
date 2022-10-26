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
    CurrentAmmo=DefaultAmmo;
}

void ABaseWeapon::StartFire()
{
}

void ABaseWeapon::StopFire()
{}

void ABaseWeapon::MakeShot()
{
}

APlayerController * ABaseWeapon::GetPlayerController() const
{
    const auto Player=Cast<ACharacter>(GetOwner());
    if(!Player)return nullptr;
    
    const auto Controller=Player->GetController<APlayerController>();
    if(!Controller)return nullptr;
    return Controller;
}

FVector ABaseWeapon::GetMuzzleLocation() const
{
    return  WeaponMesh->GetSocketLocation(SocketName);
}

void ABaseWeapon::DecreaseBullets()
{
    CurrentAmmo.Bullets--;
    Logs();
    if (IsClipEmpty()&&!IsNoAmmo())
    {
        DecreaseClip();
    }
}

void ABaseWeapon::DecreaseClip()
{
    CurrentAmmo.Bullets=DefaultAmmo.Bullets;
    if (!CurrentAmmo.bInfiniteWeapon)
    {
        CurrentAmmo.Clips--;
        UE_LOG(BaseWeaponLog,Warning,TEXT("Reloading"));
    }
}

bool ABaseWeapon::IsNoAmmo()
{
    return !CurrentAmmo.bInfiniteWeapon&&IsClipEmpty()&&CurrentAmmo.Clips==0;
}

bool ABaseWeapon::IsClipEmpty()
{
    return CurrentAmmo.Bullets==0;
}

void ABaseWeapon::Logs()
{
    FString LogAmmo="Ammo: Bullets:"+FString::FromInt(CurrentAmmo.Bullets)+"Clips: ";
    LogAmmo+=CurrentAmmo.bInfiniteWeapon?"Infinite":FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(BaseWeaponLog,Warning,TEXT("%s"),*LogAmmo);
}
