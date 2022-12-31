// Fill out your copyright notice in the Description page of Project Settings.


#include "STUSpectatorWidget.h"

#include "Chaos/MassProperties.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/Components/STURespawnComponentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSpectatorWidget, All, All)

bool USTUSpectatorWidget::GetRespawnTime(int32 &CountDownTime) const
{
    const auto Player=GetOwningPlayer();
    if(!Player)
    {
        UE_LOG(LogSTUSpectatorWidget, Error, TEXT("RespawnRequest No Player"));
        return false;
    }
    const auto Component = Player->GetComponentByClass(USTURespawnComponentComponent::StaticClass());
    if(!Component)
    {
        UE_LOG(LogSTUSpectatorWidget, Error, TEXT("RespawnRequest No Component"));
        return false;
    }
    const auto RespawnComponent=Cast<USTURespawnComponentComponent>(Component);
    if(!RespawnComponent)
    {
        UE_LOG(LogSTUSpectatorWidget, Error, TEXT("RespawnRequest No RespawnComponent"));
        return false;
    }
    if(!RespawnComponent||!RespawnComponent->IsRespawnInProgress())return false;
    
    
    
    
    CountDownTime=RespawnComponent->GetRespawnCountDown();
    return true;
}
