// Fill out your copyright notice in the Description page of Project Settings.

#include "STUPlayerController.h"

#include "ShootThemUp/Components/STURespawnComponentComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
   STURespawnComponentComponent=CreateDefaultSubobject<USTURespawnComponentComponent>("Respawn Component");
}
