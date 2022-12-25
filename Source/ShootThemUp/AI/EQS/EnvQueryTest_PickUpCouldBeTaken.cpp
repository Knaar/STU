// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_PickUpCouldBeTaken.h"

#include "BasePickUpGift.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_PickUpCouldBeTaken::UEnvQueryTest_PickUpCouldBeTaken(const FObjectInitializer &ObjectInitializer)
    :Super(ObjectInitializer)
{
    Cost=EEnvTestCost::Low;
    ValidItemType=UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickUpCouldBeTaken::RunTest(FEnvQueryInstance &QueryInstance) const
{
    const auto DataOwner=QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);
    bool WantsBeTakeble= BoolValue.GetValue();
    
   for(FEnvQueryInstance::ItemIterator It(this,QueryInstance); It; ++It)
   {
       const auto ItemActor=GetItemActor(QueryInstance,It.GetIndex());
       const auto PickupActor=Cast<ABasePickUpGift>(ItemActor);
       if(!PickupActor) continue;

       const auto CouldBeTaken=PickupActor->CouldBeTaken();
       It.SetScore(TestPurpose,FilterType ,CouldBeTaken, WantsBeTakeble);
   }
}
