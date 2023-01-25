// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "ShootThemUp/STUGameInstance.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All)

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
    InitLevelItems();
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation *Animation)
{

    Super::OnAnimationFinished_Implementation(Animation);
    if(Animation!= LoadGameAnimation)return;

    const auto STUGameInstance = GetSTUGameInstance();
    if(!STUGameInstance) return;

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnStartGame()
{
    PlayAnimation(LoadGameAnimation);
    UGameplayStatics::PlaySound2D(GetWorld(),StartGameSound);
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this,GetOwningPlayer(),EQuitPreference::Quit,true);
}

void USTUMenuWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance();
    if(!STUGameInstance||!LevelItemsBox) return;

    checkf(STUGameInstance->GetLevelsData().Num()!=0, TEXT("Levels data must not be empty"));
    LevelItemsBox->ClearChildren();

    for(auto LevelData: STUGameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(),LevelItemWidgetClass);
        if(!LevelItemWidget) continue;

        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);
    }

    if(STUGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetStartupLevel());
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData &Data)
{
    const auto STUGameInstance = GetSTUGameInstance();
    if(!STUGameInstance) return;

    STUGameInstance->SetStartupLevel(Data);

    for(auto LevelItemWidget: LevelItemWidgets)
    {
        if(LevelItemWidget)
        {
            const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USTUGameInstance * USTUMenuWidget::GetSTUGameInstance() const
{
    if(!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
    
}
