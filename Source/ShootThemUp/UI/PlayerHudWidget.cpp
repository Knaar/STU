 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"

#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"

float UPlayerHudWidget::GetHealth()
{
    const auto HealthComponent=GetHealthComponent();
    if(!HealthComponent) return 0.0f;
    return HealthComponent->GetHealthPercent();
}

bool UPlayerHudWidget::GetWeaponImage(FMyWeaponImageData &ImgData)
{
    const auto WeaponComponent=GetWeaponComponent();
    if(!WeaponComponent) return false;
    return WeaponComponent->GetMyWeaponImageData(ImgData);
}

bool UPlayerHudWidget::GetWeaponAmmoData(FMyAmmo &AmmoData)
{
    const auto WeaponComponent=GetWeaponComponent();
    if(!WeaponComponent) return false;
    return WeaponComponent->GetMyWeaponAmmo(AmmoData);
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

USTUHeathComponent * UPlayerHudWidget::GetHealthComponent() const
{
    const auto Player=GetOwningPlayerPawn();
    if(!Player)return nullptr;

    const auto Component=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent=Cast<USTUHeathComponent>(Component);
    if(!HealthComponent)return nullptr;

    return HealthComponent;
}

bool UPlayerHudWidget::IsPlayerAlive()
{
    const auto HealthComponent=GetHealthComponent();
    return HealthComponent&&!HealthComponent->IsDead();
}

bool UPlayerHudWidget::IsPlayerSpectator()
{
    const auto Controller=GetOwningPlayer();
    return Controller&&Controller->GetStateName()==NAME_Spectating;
}

void UPlayerHudWidget::NativeOnInitialized()
{
    if(GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    
    Super::NativeOnInitialized();
}

 void UPlayerHudWidget::OnNewPawn(APawn *NewPawn)
 {
    if(!NewPawn)return;
    
    const auto Component=NewPawn->GetComponentByClass(USTUHeathComponent::StaticClass());
    if(!Component)return;
    
    const auto HealthComponent=Cast<USTUHeathComponent>(Component);
    
    if(HealthComponent && !HealthComponent->OnPlayerDamaged.IsBoundToObject(this))
    {
        HealthComponent->OnPlayerDamaged.AddUObject(this,&UPlayerHudWidget::OnHealthChanged);
    }
    
 }

 void UPlayerHudWidget::OnHealthChanged(float Health,float DeltaHealth)
{
    if(DeltaHealth<0)
    {
        OnTakeDamage();
    }
    
}
