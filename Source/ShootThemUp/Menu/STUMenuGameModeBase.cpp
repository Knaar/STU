// Fill out your copyright notice in the Description page of Project Settings.


#include "STUMenuGameModeBase.h"

#include "STUMenuHUD.h"
#include "STUMenuPlayerController.h"

ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
    PlayerControllerClass = ASTUMenuPlayerController::StaticClass();
    HUDClass = ASTUMenuHUD::StaticClass();
}
