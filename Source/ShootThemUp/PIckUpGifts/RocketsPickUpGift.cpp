#include "RocketsPickUpGift.h"

#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"
#include "ShootThemUp/Weapon/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(GiftRockets, All, All);

bool ARocketsPickUpGift::IsGiftPickedUp(AActor *Actor)
{
    const auto Player=Actor;
    if(!Player)return false;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto ComponentW=Player->GetComponentByClass(UWeaponComponent::StaticClass());
    
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    if(!HealthComponent||HealthComponent->IsDead())return false;
    
    const auto WeaponComponent=Cast<UWeaponComponent>(ComponentW);
    if(!WeaponComponent||WeaponComponent->CurrentWeapon->IsAmmoFull())return false;

    WeaponComponent->TryToAddBullets(GiftAmmo,BaseWeaponToAddAmmo);
    
    UE_LOG(GiftRockets,Warning,TEXT("Gift Rocket PickedUp"));
    return true;
}
