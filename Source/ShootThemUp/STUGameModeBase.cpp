// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameModeBase.h"

#include "AIController.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "ShootThemUp/UI/GameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModBase, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = AGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnBots();

    CurrentRound = 1;
    StartRound();
}

UClass *ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController *InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::StartRound()
{
    //Сперва узнаём продолжительность раунда из нашего FGameDaa
    RoundCountDown = GameData.RoundTime;

    //Потом запускаем таймер, он будет вызывать функцию, срабатывающую каждую секунду
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModBase, Display, TEXT("Time: %i / Round: %i /%i"), RoundCountDown,CurrentRound, GameData.RoundsNum)
    //Каждую секунду будет снижать RoundCountDown
    if (--RoundCountDown == 0)
    {
        //если время на раунд = 0 надо остановить таймер
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        //Теперь у нас либо продолжается игра и запускается новый раунд, либо нет
        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            //если до, то увеличивает счетчик раундов и запускает таймер заново
            ++CurrentRound;
            StartRound();
        }
        else
        {
            //тут логика окончания игры
            UE_LOG(LogSTUGameModBase, Display, TEXT("=====GAME OVER====="))
        }
    }

}

void ASTUGameModeBase::SpawnBots()
{
    //проверяем валидность гетворлд
    if (!GetWorld())
        return;

    //спавним столько раз, сколько у нас должно быть игроков (минус один-наш плеер)
    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        //Нужна переменная FActorSpawnParameters. в ней определяем, как должны обрабатываться коллизии при спавне
        FActorSpawnParameters SpawnInfo;

        /*Для этой цели существует поле SpawnCollisionHandlingOverride
         *Поле представляет собой Enum, элементы которого определяют, каким образом будут обрабатываться коллизии*/
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        /*И спавним самого чувака(но не актора или чарактера, а его контроллер)
         *Но не просто спавним, а создаём указатель, который указывает на текущего заспавненного актора*/
        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

        //Далее для респавна будем использовать родную функция RestartPlayer
        RestartPlayer(STUAIController);

    }
}
