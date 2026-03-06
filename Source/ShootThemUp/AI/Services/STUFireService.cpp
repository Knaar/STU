// Fill out your copyright notice in the Description page of Project Settings.


#include "STUFireService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShootThemUp/AI/STUAIWeaponComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"

namespace
{
struct FSTUFireServiceMemory
{
    TWeakObjectPtr<AActor> CurrentEnemy;
    float EnemyFoundTime = 0.0f;
};
}

USTUFireService::USTUFireService()
{
    NodeName="Shoot";
}

uint16 USTUFireService::GetInstanceMemorySize() const
{
    return sizeof(FSTUFireServiceMemory);
}

void USTUFireService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    constexpr float AimDelayScale = 3.0f;

    const auto Controller=OwnerComp.GetAIOwner();
    const auto BlackBoard=OwnerComp.GetBlackboardComponent();
    const auto FireMemory = reinterpret_cast<FSTUFireServiceMemory*>(NodeMemory);
    if(!FireMemory) return;

    if (!Controller || !BlackBoard)
    {
        Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
        return;
    }

    const auto Pawn=Controller->GetCharacter();
    if (!Pawn) return;
    
    const auto EnemyActor = Cast<AActor>(BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    const bool HasAim = EnemyActor != nullptr;

    if(Controller)
    {
        const auto Component=Pawn->GetComponentByClass(UWeaponComponent::StaticClass());
        const auto WeaponComponent=Cast<UWeaponComponent>(Component);
       
        if(WeaponComponent)
        {
            if (!HasAim)
            {
                FireMemory->CurrentEnemy = nullptr;
                FireMemory->EnemyFoundTime = 0.0f;
                WeaponComponent->StopFire();
            }
            else
            {
                const float CurrentTime = Pawn->GetWorld()->GetTimeSeconds();
                if (FireMemory->CurrentEnemy.Get() != EnemyActor)
                {
                    FireMemory->CurrentEnemy = EnemyActor;
                    FireMemory->EnemyFoundTime = CurrentTime;
                }

                const float EffectiveAimDelay = AimDelay / AimDelayScale;
                const bool bCanShootNow = (CurrentTime - FireMemory->EnemyFoundTime) >= EffectiveAimDelay;
                bCanShootNow ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
            }
        }
        else
        {
            UE_LOG(LogTemp,Warning,TEXT("WeaponComponentNotFound"));
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
