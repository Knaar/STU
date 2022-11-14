#include "STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFXComponent::BeginPlay()
{
    Super::BeginPlay();

}


void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USTUWeaponFXComponent::PlayUNiagaraSystemReleased(const FHitResult & HitResult)
{
    auto ImpactData=DefaultImpactData;
    if(HitResult.PhysMaterial.IsValid())
    {
        auto Mat=HitResult.PhysMaterial.Get();
        if(ImpactDataMap.Contains(Mat))
        {
            ImpactData=ImpactDataMap[Mat];
        }
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ImpactData.NiagaraEffect,HitResult.ImpactPoint,HitResult.ImpactNormal.Rotation());

    auto DecalComponent=UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        ImpactData.DecalData.Material,
        ImpactData.DecalData.Size,
        HitResult.ImpactPoint,
        HitResult.ImpactNormal.Rotation());

    if(DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime,ImpactData.DecalData.FadeOutTime);
    }
}
