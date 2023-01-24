// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "STUBaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();
    

    GameWidgets.Add(ESTUMatchState::InProgress,CreateWidget<USTUBaseWidget>(GetWorld(),PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(),PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(),GameOverWidgetClass));

    for(auto GameWidgetPair:GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if(!GameWidget)continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
        }
    }
}

void AGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if(GameWidgets.Contains(State))
    {
        CurrentWidget=GameWidgets[State];
    }

    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state chsnged: %s"), *UEnum::GetValueAsString(State))
}


