// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget,All, All)

void USTUGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if(GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMainMenu);
    }
}

void USTUGoToMenuWidget::OnMainMenu()
{
    if(!GetWorld())return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!STUGameInstance) return;

    if(STUGameInstance->GetMainMenuLevelName().IsNone())
    {
        UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this,STUGameInstance->GetMainMenuLevelName());
}

