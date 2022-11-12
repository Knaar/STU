#include "STUHeathComponent.h"

#include "ShootThemUp/Dev/ColdDamageType.h"
#include "ShootThemUp/Dev/FireDamageType.h"


USTUHeathComponent::USTUHeathComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void USTUHeathComponent::BeginPlay()
{
    Super::BeginPlay();

    //через GetOwner получаем доступ к Character
    AActor* ActorPTr=GetOwner();

    //Если Character доступен-подписываемся на родной делегат OnTakeAnyDamage
    if(ActorPTr)
    {
        ActorPTr->OnTakeAnyDamage.AddDynamic(this,&ThisClass::OnTakeAnyDamage);
    }

    //задаём количество ХП
    SetHealth(MaxHealth);
}

void USTUHeathComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTUHeathComponent::AutoHeal()
{
    SetHealth(CurrentHealth+HealModifier);
    
    if (IsHealthFull()&&GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }
    
}

void USTUHeathComponent::SetHealth(float Health)
{
    CurrentHealth=FMath::Clamp(Health,0.0f,MaxHealth);
    OnPlayerDamaged.Broadcast(CurrentHealth);
}

void USTUHeathComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                         AController *InstigatedBy, AActor *DamageCauser)
{
    //проверяем на нулевой урон и состояние жизни
    if (Damage<=0||IsDead()||!GetWorld()){return;}

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    SetHealth(CurrentHealth-Damage);
            
    if (IsDead())
    {
        OnPlayerDeath.Broadcast();
        bAutoHealEnabled=false;
    }
    else if(bAutoHealEnabled)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&USTUHeathComponent::AutoHeal,HealUpdateTime,true,FirstHealDelay);
    }
}

bool USTUHeathComponent::IsHealthFull()
{
    return FMath::IsNearlyEqual(CurrentHealth,MaxHealth);
}

bool USTUHeathComponent::TryToAddHealth(float Health)
{
    if(!IsHealthFull()&&!IsDead()&&Health>0)
    {
        SetHealth(CurrentHealth+Health);
        return true;
    }
    return false;
}

/*Блок кода, для проверки на тип урона
    if (DamageType)
    {
        if (DamageType->IsA<UFireDamageType>())
        {
            UE_LOG(LogTemp,Warning,TEXT("So Hot!"));
        }
        else if(DamageType->IsA<UColdDamageType>())
        {
            UE_LOG(LogTemp,Warning,TEXT("So Cold"));
        }
    }*/