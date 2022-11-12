#include "STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

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

void USTUWeaponFXComponent::PlayUNiagaraSystemReleased(FHitResult &HitResult)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NiagaraEmitter,HitResult.ImpactPoint,HitResult.ImpactNormal.Rotation());
   
}
