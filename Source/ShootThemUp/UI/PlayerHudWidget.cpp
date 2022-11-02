// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"
#include "Components/STUHeathComponent.h"
#include "Components/WeaponComponent.h"

float UPlayerHudWidget::GetHealth()
{
    const auto Player=GetOwningPlayerPawn();
    if(!Player)return 0.0f;

    const auto Component=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent=Cast<USTUHeathComponent>(Component);
    if(!HealthComponent)return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool UPlayerHudWidget::GetWeaponImage(FMyWeaponImageData &ImgData)
{
    return GetWeaponComponent()->GetMyWeaponImageData(ImgData);
}

bool UPlayerHudWidget::GetWeaponAmmoData(FMyAmmo &AmmoData)
{
        
    return GetWeaponComponent()->GetMyWeaponAmmo(AmmoData);
}

UWeaponComponent* UPlayerHudWidget::GetWeaponComponent() const
{
    const auto Player=GetOwningPlayerPawn();
    if(!Player)return nullptr;

    const auto Component=Player->GetComponentByClass(UWeaponComponent::StaticClass());
    const auto WeaponComponent=Cast<UWeaponComponent>(Component);
    if(!WeaponComponent)return nullptr;

    return WeaponComponent;
}
