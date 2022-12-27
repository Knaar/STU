#include "HealthPickUpGift.h"

#include "ShootThemUp/Components/STUHeathComponent.h"


DEFINE_LOG_CATEGORY_STATIC(GiftHealth, All, All);

bool AHealthPickUpGift::GivePickUpTo(APawn *Pawn)
{
    const auto HealthComponent=GetHealthComponent(Pawn);
    
    if(!HealthComponent||HealthComponent->IsHealthFull()||HealthComponent->IsDead())return false;

    HealthComponent->TryToAddHealth(HealthAmount);
    
    return true;
}
