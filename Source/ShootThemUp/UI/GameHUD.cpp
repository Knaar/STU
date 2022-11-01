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

void AGameHUD::DrawHUD()
{
    Super::DrawHUD();
    //DrawLineFunc();
}

void AGameHUD::DrawLineFunc()
{
     int32 SizeX = Canvas->SizeX;
     int32 SizeY = Canvas->SizeY;
    
    const TInterval<float> Center (SizeX*0.5f,SizeY*0.5f);

    float HalfLineSize=10.0f;
    float LineThickness=2.0f;
    FLinearColor LineColor=FLinearColor::Green;
    
    DrawLine(Center.Min-HalfLineSize,Center.Max,Center.Min+HalfLineSize,Center.Max,LineColor,LineThickness);
    DrawLine(Center.Min,Center.Max-HalfLineSize,Center.Min,Center.Max+HalfLineSize,LineColor,LineThickness);
}
