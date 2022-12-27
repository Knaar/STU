#include "RocketsPickUpGift.h"

#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"
#include "ShootThemUp/Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(GiftRockets, All, All);

USTUHeathComponent* ARocketsPickUpGift::GetHealthComponent(APawn *Pawn)
{
    const auto Player=Pawn;
    if(!Player)return nullptr;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    return HealthComponent;
}

bool ARocketsPickUpGift::GivePickUpTo(APawn *Pawn)
{
    const auto Player=Pawn;
    if(!Player)return false;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto ComponentW=Player->GetComponentByClass(UWeaponComponent::StaticClass());
    
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    if(!HealthComponent||HealthComponent->IsDead())return false;
    
    const auto WeaponComponent=Cast<UWeaponComponent>(ComponentW);
    
    if(!WeaponComponent) return false; //||WeaponComponent->CurrentWeapon->IsAmmoFull())return false;

    if(IsNeedToPickUp(Pawn))
    {
        WeaponComponent->TryToAddBullets(ClipsAmount,BaseWeaponToAddAmmo);
        UE_LOG(GiftRockets,Warning,TEXT("Gift Rocket PickedUp"));
        return true;
    }
    else
    {
        return false;
    }
}

bool ARocketsPickUpGift::IsNeedToPickUp(APawn *Pawn) 
{
    const auto Player=Pawn;
    if(!Player)return false;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto ComponentW=Player->GetComponentByClass(UWeaponComponent::StaticClass());
    
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    if(!HealthComponent||HealthComponent->IsDead())return false;
    
    const auto WeaponComponent=Cast<UWeaponComponent>(ComponentW);

    for (const auto WeaponsRunner:WeaponComponent->Weapons)
    {
        const auto bIsAmmoFull= WeaponsRunner->IsAmmoFull();
        if(!bIsAmmoFull)
        {
            return true;
        }
    }
    return false;
}
