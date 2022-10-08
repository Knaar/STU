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
