// Fill out your copyright notice in the Description page of Project Settings.

#include "STUGameModeBase.h"
#include "AIController.h"
#include "STUPlayerState.h"
#include "Components/STURespawnComponentComponent.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "ShootThemUp/UI/GameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModBase, All, All)

//переменная минимального значения остатка времени для респавна
constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = AGameHUD::StaticClass();
    //Оказывается, прямо здесь можно задать базовый PlayerState
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnBots();

    //Ботов создали, теперь распределяем по командам
    CreateTeamsInfo();

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
    UE_LOG(LogSTUGameModBase, Display, TEXT("Time: %i / Round: %i /%i"), RoundCountDown, CurrentRound,
           GameData.RoundsNum)
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
            ResetPlayers();
            StartRound();
        }
        else
        {
            //тут логика окончания игры
            UE_LOG(LogSTUGameModBase, Display, TEXT("=====GAME OVER====="))
            LogPlayerInfo();
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

void ASTUGameModeBase::ResetPlayers()
{
    //Нам потребуется указатель на UWorld. поэтому проверяем GetWorld()
    if (!GetWorld())
        return;

    //Информацию о всех контроллерах в мире можно получить через специальный итератор. Мы создадим цикл обхода всех контроллеров
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        //Теперь каждого индивидуально ресетим. Причем, контроллер мы получаем, просто  нажав Get()
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController *Controller)
{
    if (Controller && Controller->GetPawn())
    {
        //Родной функционал, способный ресетнуть Контроллер и, вместе с ним, павна. Не работает без PlayerState 
        Controller->GetPawn()->Reset();
    }
    //Тут уже вызываем функцию гейммода RestartPlayer(), и передаём указатель на контроллер
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld())
        return;

    int32 TeamId = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        //"сырой" указатель на контроллер
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;

        PlayerState->SetTeamID(TeamId);
        PlayerState->SetTeamColor(DetermineColorByTeamId(TeamId));
        SetPlayerColor(Controller);

        TeamId = TeamId == 1? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamId(int32 TeamId) const
{
    if(TeamId-1<GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamId-1];
    }
    UE_LOG(LogSTUGameModBase,Warning,TEXT("No color for team: %i, set to default: %s"), TeamId, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController *Controller)
{
    if(!Controller) return;

    const auto Character= Cast<ASTUBaseCharacter>(Controller->GetCharacter());
    if(!Character) return;

    const auto PlayerState=Cast<ASTUPlayerState>(Controller->PlayerState);
    if(!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController *KillerController, AController *VictimController)
{
    const auto KillerPlayerState = KillerController? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if(KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if(VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }
    StartRespawn(VictimController);
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld())
        return;

    
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        //"сырой" указатель на контроллер
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::StartRespawn(AController *Controller)
{
    //Булевая, котора будет проверять, возможен ли рескпавн
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if(!RespawnAvailable)return;
    
    const auto Component=Controller->GetComponentByClass(USTURespawnComponentComponent::StaticClass());
    if(!Component)
    {
        UE_LOG(LogSTUGameModBase, Display, TEXT("RespawnRequest No Component"));
        return;
    }
    const auto RespawnComponent=Cast<USTURespawnComponentComponent>(Component);
    if(!RespawnComponent)
    {
        UE_LOG(LogSTUGameModBase, Display, TEXT("RespawnRequest No RespawnComponent"));
        return;
    }
    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::RespawnRequest(AController *Controller)
{
    ResetOnePlayer(Controller);
}


