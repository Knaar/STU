// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameOverWidget.h"

#include "STUPlayerState.h"
#include "STUPlayerStatRowWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/STUGameInstance.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "ShootThemUp/STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameOverWidget,All,All)

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
        }
    }
    if(ResetLevelButton)
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &ThisClass::USTUGameOverWidget::OnResetLevel);
    }
    
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if(State==ESTUMatchState::GameOver)
    {
        UpdatePlayerStat();
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if(!GetWorld() || !PlayerStatBox) return;

    PlayerStatBox->ClearChildren();
    
    //Создаём цикл на все контроллеры
    for(auto It= GetWorld()->GetControllerIterator(); It ; ++It)
    {
        const auto Controller=It->Get();
        if(!Controller)continue;

        const auto PlayerState=Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState)continue;

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
        if(!PlayerStatRowWidget)continue;

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamId()));
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatBox->AddChild(PlayerStatRowWidget);
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const auto CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this); 
    UGameplayStatics::OpenLevel(this,FName(CurrentLevelName));
}

