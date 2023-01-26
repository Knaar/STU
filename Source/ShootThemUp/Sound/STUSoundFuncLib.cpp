// Fill out your copyright notice in the Description page of Project Settings.


#include "STUSoundFuncLib.h"
#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUSoundFuncLib,All,All);

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass *SoundClass, float Volume)
{
    if(!SoundClass)return;

    SoundClass->Properties.Volume = FMath::Clamp(Volume,0.0f,1.0f);
    UE_LOG(LogSTUSoundFuncLib,Display,TEXT("Sound class Volume was changed: %s = %f"),*SoundClass->GetName(),
        SoundClass->Properties.Volume)
}

void USTUSoundFuncLib::ToggleSoundClassVolume(USoundClass *SoundClass)
{
    if(!SoundClass) return;

    const auto NextVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
    SetSoundClassVolume(SoundClass,NextVolume);
}

float USTUSoundFuncLib::GetSoundClassVolume(USoundClass *SoundClass)
{
    if(!SoundClass) return NULL;
    return SoundClass->Properties.Volume;
    
}
