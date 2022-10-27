// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimNotify.h"

void UBaseAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    //Super::Notify(MeshComp, Animation);
    OnNotify.Broadcast(MeshComp);
}
