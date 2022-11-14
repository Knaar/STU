#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "STUWeaponFXComponent.generated.h"


class UNiagaraSystem;
class UPhysicalMaterial;

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    UMaterialInstance* Material;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    FVector Size=FVector(10.0f);

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    float LifeTime=50.0f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    float FadeOutTime=0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    FDecalData DecalData;
};

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
    FImpactData DefaultImpactData;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="VFX")
    TMap<UPhysicalMaterial*, FImpactData>ImpactDataMap;
    
    void PlayUNiagaraSystemReleased(const FHitResult& HitResult);
};
