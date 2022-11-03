#include "HealthPickUpGift.h"

#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GiftHealth, All, All);

bool AHealthPickUpGift::IsGiftPickedUp(AActor *Actor)
{
    const auto Player=Actor;
    if(!Player)return false;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto ComponentW=Actor->GetComponentByClass(UWeaponComponent::StaticClass());

    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    const auto WeaponComponent=Cast<UWeaponComponent>(ComponentW);

    if(!HealthComponent||HealthComponent->IsDead())return false;

    HealthComponent->SetHealth(SetHealthAmount);
    UE_LOG(GiftHealth,Warning,TEXT("Gift Health PickedUp"));
    return true;
}
