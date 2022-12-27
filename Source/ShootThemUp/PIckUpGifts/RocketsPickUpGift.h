#pragma once

#include "CoreMinimal.h"
#include "BasePickUpGift.h"
#include "RocketsPickUpGift.generated.h"
class USTUHeathComponent;
class ASTUBaseWeapon;


UCLASS()
class SHOOTTHEMUP_API ARocketsPickUpGift : public ABasePickUpGift
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="PickUp",meta=(ClampMin="1.0",ClampMax="10.0"))
    int32 ClipsAmount;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="PickUp")
    TSubclassOf<ASTUBaseWeapon> BaseWeaponToAddAmmo;

    UFUNCTION()
    USTUHeathComponent* GetHealthComponent(APawn* Pawn);
    
    UFUNCTION()
    UWeaponComponent* GetWeaponComponent(APawn* Pawn);
    
    virtual bool GivePickUpTo(APawn *Pawn) override;

    bool IsNeedToPickUp(APawn* Pawn);

    
};
