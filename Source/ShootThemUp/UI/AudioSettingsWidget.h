// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingsWidget.generated.h"

class USlider;
class UTextBlock;
class USoundClass;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatValueChangedEvent, float, Value);

UCLASS()
class SHOOTTHEMUP_API UAudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
    
public:
    //FOnFloatValueChangedEvent Event;
    virtual void NativeOnInitialized() override;
    
    UPROPERTY(meta=(BindWidget))
    USlider* SoundSlider;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* NameOfCue;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
    USoundClass* SoundClass;

    UFUNCTION(BlueprintCallable)
    void SetNewValueToSoundClass(float Value);

    UFUNCTION()
    void OnSet(float Value);
};
