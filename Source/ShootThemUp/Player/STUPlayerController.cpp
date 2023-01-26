// Fill out your copyright notice in the Description page of Project Settings.

#include "STUPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "ShootThemUp/STUGameInstance.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "ShootThemUp/Components/STURespawnComponentComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
   STURespawnComponentComponent=CreateDefaultSubobject<USTURespawnComponentComponent>("Respawn Component");
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);
    OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if(!InputComponent)return;

    InputComponent->BindAction("PauseGame",IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
    InputComponent->BindAction("MuteSounds",IE_Pressed, this, &ASTUPlayerController::OnMuteSound);
}

void ASTUPlayerController::OnPauseGame()
{
    if(!GetWorld()||!GetWorld()->GetAuthGameMode())return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    
    if(State==ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor=false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor=true;
    }
}

void ASTUPlayerController::OnMuteSound()
{
    const auto GameInstance=GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!GameInstance) return;
    
    GameInstance->ToggleVolume();
}
