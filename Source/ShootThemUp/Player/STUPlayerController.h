// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShootThemUp/STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponentComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()
    ASTUPlayerController();
public:
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
    USTURespawnComponentComponent* STURespawnComponentComponent;
protected:

    virtual void BeginPlay() override;
    
    virtual void OnPossess(APawn *InPawn) override;

    /*БлокПаузы в игре
    *эта часть необходима для получения доступа к биндингу.
    *Паузу реализуем в контроллере, так как она должна быть доступна, даже если игрок погиб
    */

    //Функция для биндинга плеер контроллера
    virtual void SetupInputComponent() override;

private:
    //CallBack функция, срабатывающая при нажатии на клавишу "Р"
    void OnPauseGame();

    //Callback Функция срабатывающая при изменении State игры
    void OnMatchStateChanged(ESTUMatchState STate);
};
