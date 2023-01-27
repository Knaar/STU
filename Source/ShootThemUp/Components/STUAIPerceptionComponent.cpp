// Fill out your copyright notice in the Description page of Project Settings.


#include "STUAIPerceptionComponent.h"

//Модуль даёт обзор
#include "AIController.h"
#include "STUHeathComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

#include "ShootThemUp/STUUtils.h"

AActor * USTUAIPerceptionComponent::GetClosestEnemy() const
{
    //массив, в котором будут все видимые акторы
    TArray<AActor*> PercieveActors;

    //функция, которая заполнит наш массив, если НПС кого-либо увидит
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(),PercieveActors);
    if(PercieveActors.Num()==0)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(),PercieveActors);
        if(PercieveActors.Num()==0)
        {
            return nullptr;
        }
      
    }
    //Кастимся к контроллеру через владельца
    const auto Controller=Cast<AAIController>(GetOwner());
    if(!Controller)return nullptr;

    //Кастимся к павну через контроллер-гет павн
    const auto Pawn=Controller->GetPawn();
    if(!Pawn)return nullptr;

    float BestDistance=MAX_FLT;
    AActor* BestPawn=nullptr;

    for(const auto PercieveActor:PercieveActors)
    {
        const auto Component=PercieveActor->GetComponentByClass(USTUHeathComponent::StaticClass());
        const auto HealthComponent=Cast<USTUHeathComponent>(Component);

        const auto PercievePawn=Cast<APawn>(PercieveActor);
        const auto AreEnemies=PercievePawn && STUUtils::AreEnemies(Controller,PercievePawn->Controller);
        
        if(HealthComponent&&!HealthComponent->IsDead() && AreEnemies)
        {
            const auto Difference=PercieveActor->GetActorLocation() - Pawn->GetActorLocation();
            const auto CurrentDistance=Difference.Size();
            if(CurrentDistance<BestDistance)
            {
                BestDistance=CurrentDistance;
                BestPawn=PercieveActor;
            }
        }
    }
    return BestPawn;
}
