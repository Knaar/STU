#include "STUProjectile.h"

#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SetRootComponent(SphereCollision);
    SphereCollision->InitSphereRadius(5.0f);
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
}
