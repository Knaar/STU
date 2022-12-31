// Fill out your copyright notice in the Description page of Project Settings.


#include "STURespawnComponentComponent.h"

#include "ShootThemUp/STUGameModeBase.h"


USTURespawnComponentComponent::USTURespawnComponentComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

}

void USTURespawnComponentComponent::Respawn(int32 RespawnTime)
{
    //e компонента нет таймер менеджера. Его можно получить через UWorld
    if(!GetWorld())return;

    RespawnCountDown=RespawnTime;

    //принцип работы похож на луп. он будет без первоначальной задержки запускать свой функционал, пока его не обнулят
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle,this,&ThisClass::RespawnTimerUpdate,1.0f,true);
}

void USTURespawnComponentComponent::RespawnTimerUpdate()
{
    //убираем секунду
    if(--RespawnCountDown==0)
    {
        //останавливаем таймер
        if(!GetWorld())return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode=Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(!GameMode)return;

        //вызываем у гейммода функцию RespawnRequest(). Передаём указатель на контроллер владельца
        GameMode->RespawnRequest(Cast<AController>(GetOwner()))
         ;
    }
}

bool USTURespawnComponentComponent::IsRespawnInProgress() const
{
    return GetWorld()&&GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}


