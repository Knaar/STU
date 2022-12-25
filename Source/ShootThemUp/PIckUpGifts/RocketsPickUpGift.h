#pragma once

#include "CoreMinimal.h"
#include "BasePickUpGift.h"
#include "RocketsPickUpGift.generated.h"
class ASTUBaseWeapon;


UCLASS()
class SHOOTTHEMUP_API ARocketsPickUpGift : public ABasePickUpGift
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GiftAmmo",meta=(ClampMin=0,ClampMax=10))
    int32 GiftAmmo;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GiftAmmo")
    TSubclassOf<ASTUBaseWeapon> BaseWeaponToAddAmmo;
        
    virtual bool IsGiftPickedUp(AActor *Actor) override;
};
