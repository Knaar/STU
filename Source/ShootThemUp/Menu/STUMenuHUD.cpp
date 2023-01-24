// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "ShootThemUp/UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    if(MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(),MenuWidgetClass);
        if(MenuWidget)
        {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}
