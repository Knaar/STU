// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHeathComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPllayerDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerDamagedSignature, float,float);


class UPhysicalMaterial;

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

    //Материал
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Health")
    TMap<UPhysicalMaterial*, float> DamageModifiers;
    //Камера Шейкер
    
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="CameraShaker")
    TSubclassOf<UCameraShakeBase> CameraShake;
private:
    void ShakeTheCameraOnDamage();
    
    //Геттер на хп

public:
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

    //Смерть
    void Killed(AController* KillerController);

    //хедшот
    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser );

    void ApplyDamage(float  Damage, AController* InstigatedBy);

    float GetPointDamageModifier(AActor* DamageActor, const FName& BoneName);

    //получение инфы об обидчике

    void ReportDamageEvent(float Damage, AController* InstigatedBy);
};
