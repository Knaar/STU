// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget,All,All)

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &ThisClass::USTUMenuWidget::OnStartGame);
    }
    if(QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::USTUMenuWidget::OnQuitGame);
    }
}

void USTUMenuWidget::OnStartGame()
{
    if(!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!STUGameInstance) return;

    if(STUGameInstance->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogSTUMenuWidget,Error, TEXT("Level name is NONE"));
        return;
    }
    
    
    UGameplayStatics::OpenLevel(this,STUGameInstance->GetStartupLevelName());
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this,GetOwningPlayer(),EQuitPreference::Quit,true);
}
