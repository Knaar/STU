#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponentComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTURespawnComponentComponent();

    void Respawn(int32 RespawnTime);

protected:
    FTimerHandle RespawnTimerHandle;

    //Оставшееся время респауна
    int32 RespawnCountDown = 0;

    void RespawnTimerUpdate();

    //геттеры
public:
    int32 GetRespawnCountDown()const{return RespawnCountDown;}
    bool IsRespawnInProgress()const;
};
