#include "STUHeathComponent.h"

#include "GameFramework/Character.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "ShootThemUp/Dev/ColdDamageType.h"
#include "ShootThemUp/Dev/FireDamageType.h"


USTUHeathComponent::USTUHeathComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void USTUHeathComponent::BeginPlay()
{
    Super::BeginPlay();

    //задаём количество ХП
    SetHealth(MaxHealth);
    
    //через GetOwner получаем доступ к Character
    AActor* ActorPTr=GetOwner();

    //Если Character доступен-подписываемся на родной делегат OnTakeAnyDamage
    if(ActorPTr)
    {
        ActorPTr->OnTakeAnyDamage.AddDynamic(this,&ThisClass::OnTakeAnyDamage);
        ActorPTr->OnTakePointDamage.AddDynamic(this,&ThisClass::OnTakePointDamage);
        ActorPTr->OnTakeRadialDamage.AddDynamic(this,&ThisClass::OnTakeRadialDamage);
    }
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

void USTUHeathComponent::ShakeTheCameraOnDamage()
{
    if(!CameraShake||IsDead())return;
    const auto Player=Cast<APawn>(GetOwner());
    if(!Player)return;

    const auto Controller=Player->GetController<APlayerController>();
    if(!Controller||!Controller->PlayerCameraManager)return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHeathComponent::SetHealth(float Health)
{
    auto TempHP=FMath::Clamp(Health,0.0f,MaxHealth);
    auto DeltaHP=TempHP-CurrentHealth;
    CurrentHealth=TempHP;
    OnPlayerDamaged.Broadcast(CurrentHealth,DeltaHP);
}

void USTUHeathComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                         AController *InstigatedBy, AActor *DamageCauser)
{
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

void USTUHeathComponent::Killed(AController *KillerController)
{
    if(!GetWorld()) return;
    
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if(!GameMode) return;

    const auto Player= Cast<APawn>(GetOwner());
    const auto VictimController=Player? Player->Controller : nullptr;

    GameMode->Killed(KillerController,VictimController);
}

void USTUHeathComponent::OnTakePointDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
    FVector HitLocation, UPrimitiveComponent *FHitComponent, FName BoneName, FVector ShotFromDirection,
    const UDamageType *DamageType, AActor *DamageCauser)
{
    const auto FinalDamage=Damage* GetPointDamageModifier(DamagedActor,BoneName);
    ApplyDamage(FinalDamage,InstigatedBy);
}

void USTUHeathComponent::OnTakeRadialDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
    FVector Origin, FHitResult HitInfo, AController *InstigatedBy, AActor *DamageCauser)
{
    ApplyDamage(Damage,InstigatedBy);
}

void USTUHeathComponent::ApplyDamage(float Damage, AController *InstigatedBy)
{
    //проверяем на нулевой урон и состояние жизни
    if (Damage<=0||IsDead()||!GetWorld()){return;}

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

    SetHealth(CurrentHealth-Damage);
    ShakeTheCameraOnDamage();
    if (IsDead())
    {
        Killed(InstigatedBy);
        OnPlayerDeath.Broadcast();
        bAutoHealEnabled=false;
    }
    else if(bAutoHealEnabled)
    {
        
        GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&USTUHeathComponent::AutoHeal,HealUpdateTime,true,FirstHealDelay);
    }
    ShakeTheCameraOnDamage();
}

float USTUHeathComponent::GetPointDamageModifier(AActor *DamageActor, const FName &BoneName)
{
    const auto Character = Cast<ACharacter>(DamageActor);
    if(!Character||//
        !Character->GetMesh()||//
        !Character->GetMesh()->GetBodyInstance(BoneName))
            return 1.0f;

    const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if(!DamageModifiers.Contains(PhysMaterial)) return 1;

    return DamageModifiers[PhysMaterial];
}

