#include "BaseWeapon.h"


ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal mesh");
    SetRootComponent(SkeletalMesh);
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}
