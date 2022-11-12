#include "HealthPickUpGift.h"

#include "ShootThemUp/Components/STUHeathComponent.h"


DEFINE_LOG_CATEGORY_STATIC(GiftHealth, All, All);

bool AHealthPickUpGift::IsGiftPickedUp(AActor *Actor)
{
    const auto Player=Actor;
    if(!Player)return false;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    
    if(!HealthComponent||HealthComponent->IsHealthFull()||HealthComponent->IsDead())return false;

    HealthComponent->TryToAddHealth(HealthAmount);
    
    UE_LOG(GiftHealth,Warning,TEXT("Gift Health PickedUp"));
    return true;
}
