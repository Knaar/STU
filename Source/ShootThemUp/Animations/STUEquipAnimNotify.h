// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUEquipAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotify,USkeletalMeshComponent*)
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUEquipAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
    
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    
    bool Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const;

    FOnNotify OnNotify;
};
