// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHeathComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPllayerDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDamagedSignature, float);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHeathComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHeathComponent();
    
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    //Делегаты на Death и получение урона
    FOnPllayerDeathSignature OnPlayerDeath;
    FOnPlayerDamagedSignature OnPlayerDamaged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(ClampMin="0", ClampMax=100))
    float MaxHealth = 100;
    float CurrentHealth;

    //Блок автохила
    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AutoHeal")
    bool bAutoHealEnabled = true;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AutoHeal",meta=(EditCondition="bAutoHealEnabled"))
    float HealUpdateTime = 0.3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AutoHeal",meta=(EditCondition="bAutoHealEnabled"))
    float FirstHealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AutoHeal",meta=(EditCondition="bAutoHealEnabled"))
    float HealModifier = 1.0f;
    
    UFUNCTION()
    void AutoHeal();

    FTimerHandle TimerHandle;
    
    //Геттер на хп
    float GetHealth() const{return CurrentHealth;}

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const{return CurrentHealth/MaxHealth;}

    void SetHealth(float Health);

    //Связка:Мы передаём инфу каждый тик, о том что игрок получил урон. Подписываемся на родной делегат. делегат возвращает значения в функцию. 
    UFUNCTION()
    void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

    UFUNCTION(BlueprintCallable)
    bool IsDead() {return FMath::IsNearlyZero(CurrentHealth); }

    //Получение здоровья

    bool IsHealthFull();

    bool TryToAddHealth(float Health);
};
