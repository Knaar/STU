// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

/*Наиболеее предпочтительно спавнить не самих персонажей, а их контроллеры
 *У Character вызывается Destroy в случае гибели персонажа, контроллеры со сцены не удаляются,
 *они же и будут ответственны за респаун.
 */
class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    //Родная функция ГеймМода, которая вызывает бегинплей
    virtual void StartPlay() override;

    /*Родная функция гейммода, которая задаёт павна для RestartPlayer.
     *Функция Имеет прикол, у неё есть _Implementation/ это последствие BlueprintNativeEvent
     *Такой синтаксис говорит о том, что такую функцию можно переопределять и в С++ и блюпринтах*/
    virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

protected:
    //Объявляем проперти, в котором будет храниться наш класс контроллера
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<AAIController> AIControllerClass;

    //Проперти, в котором храним AICharacter для спауна
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TSubclassOf<APawn> AIPawnClass;


    //в след проперти будет храниться наш ГеймДата
    UPROPERTY(EditDefaultsOnly, Category="Game")
    FGameData GameData;

private:
    
    //переменная, которая хранит номер текущего раунда
    int32 CurrentRound = 1;

    //Остаток времени, для текущего раунда
    int32 RoundCountDown = 0;

    //Дискриптор таймера
    FTimerHandle GameRoundTimerHandle;

    //Функция Запуска раунда
    void StartRound();

    //Функция обновления таймера
    void GameTimerUpdate();
    
    //Основная функция спауна ботов
    void SpawnBots();
};
