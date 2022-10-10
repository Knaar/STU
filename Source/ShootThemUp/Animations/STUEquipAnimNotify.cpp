// Fill out your copyright notice in the Description page of Project Settings.


#include "STUEquipAnimNotify.h"

void USTUEquipAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    OnNotify.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}

bool USTUEquipAnimNotify::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
{
    OnNotify.Broadcast(MeshComp);
    Super::Received_Notify(MeshComp,Animation,EventReference);
    return true;
}
