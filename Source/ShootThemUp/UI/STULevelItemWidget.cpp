// Fill out your copyright notice in the Description page of Project Settings.


#include "STULevelItemWidget.h"

#include "NiagaraVariant.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTULevelItemWidget::SetLevelData(const FLevelData &Data)
{
    LevelData=Data;
    if(LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }

    if(LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);
    }
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
    if(LevelImage)
    {
        LevelImage->SetColorAndOpacity(IsSelected? FLinearColor::Red : FLinearColor::White );
    }
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this,&ThisClass::USTULevelItemWidget::OnLevelItemClicked);
        LevelSelectButton->OnHovered.AddDynamic(this,&ThisClass::USTULevelItemWidget::OnLevelItemHovered);
        LevelSelectButton->OnUnhovered.AddDynamic(this,&ThisClass::USTULevelItemWidget::OnLevelItemUnHovered);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::OnLevelItemHovered()
{
    if(FrameImage)
    {
        FrameImage->SetVisibility ( ESlateVisibility::Visible);
    }
}

void USTULevelItemWidget::OnLevelItemUnHovered()
{
    if(FrameImage)
    {
        FrameImage->SetVisibility (ESlateVisibility::Hidden);
    }
}
