#pragma once
#include "STUCoreTypes.generated.h"
USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta = (ClampMin= "1", ClampMax= "100"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta = (ClampMin= "1", ClampMax= "10"))
    int32 RoundsNum = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta = (ClampMin= "3", ClampMax= "300"))
    int32 RoundTime = 3;//in seconds
};
