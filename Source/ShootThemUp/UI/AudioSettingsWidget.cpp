// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSettingsWidget.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Sound/SoundClass.h"

#include <string>

void UAudioSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(!SoundClass)return;
    
    SoundSlider->SetValue(SoundClass->Properties.Volume);
    SoundSlider->OnValueChanged.AddDynamic(this,&ThisClass::UAudioSettingsWidget::OnSet);
    
    const auto GettedName = SoundClass->GetName();
    
    NameOfCue->SetText(FText::FromString(GettedName));
}

void UAudioSettingsWidget::SetNewValueToSoundClass(float Value)
{
    if(!SoundClass)return;
    SoundClass->Properties.Volume=Value;
}

void UAudioSettingsWidget::OnSet(float Value)
{
    SetNewValueToSoundClass(Value);
}

