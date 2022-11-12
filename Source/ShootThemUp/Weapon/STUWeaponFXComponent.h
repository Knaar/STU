#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
//#include "PhysicalMaterials/PhysicalMaterial.h"
#include "STUWeaponFXComponent.generated.h"


class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponFXComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    UNiagaraSystem *NiagaraEmitter;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    TMap<UPhysicalMaterial*, UNiagaraSystem*>MaterialMap;
    
    void PlayUNiagaraSystemReleased(FHitResult& HitResult);
};
