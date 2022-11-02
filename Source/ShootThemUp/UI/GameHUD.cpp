// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();
    auto TheUserWidget=CreateWidget<UUserWidget>(GetWorld(),SelectedUserWidget);
    if(TheUserWidget)
    {
        TheUserWidget->AddToViewport();
    }
}


