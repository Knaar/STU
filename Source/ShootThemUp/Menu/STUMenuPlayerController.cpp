// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuPlayerController.h"

#include "ShootThemUp/STUGameInstance.h"

void ASTUMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;

}
