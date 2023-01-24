 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHudWidget.h"

#include "STUPlayerState.h"
#include "Components/ProgressBar.h"
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
    UpdateHealthBar();
 }

 void UPlayerHudWidget::OnHealthChanged(float Health,float DeltaHealth)
{
    if(DeltaHealth<0)
    {
        OnTakeDamage();
        if(!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

 int32 UPlayerHudWidget::GetKillsNum() const
 {
    const auto Controller = GetOwningPlayer();
    if(!Controller) return 0;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState ? PlayerState->GetKillsNum() : 0;
 }

 FString UPlayerHudWidget::FormatBullets(int32 BulletsNum) const
 {
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletSTR = FString::FromInt(BulletsNum);
    const auto SymbolsNumToAdd = MaxLen - BulletSTR.Len();

    if(SymbolsNumToAdd > 0)
    {
        BulletSTR = FString::ChrN(SymbolsNumToAdd,PrefixSymbol).Append(BulletSTR);
    }
    return BulletSTR;
 }

 void UPlayerHudWidget::UpdateHealthBar()
 {
    if(HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealth()>PercentColorThreshold?GoodColor : BadColor);
    }
 }
