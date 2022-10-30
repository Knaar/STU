// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"
#include "Components/STUHeathComponent.h"

float UPlayerHudWidget::GetHealth()
{
    const auto Player=GetOwningPlayerPawn();
    if(!Player)return 0.0f;

    const auto Component=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent=Cast<USTUHeathComponent>(Component);
    if(!HealthComponent)return 0.0f;

    return HealthComponent->GetHealthPercent();
}
