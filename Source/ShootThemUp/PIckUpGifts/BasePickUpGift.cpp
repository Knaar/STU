#include "BasePickUpGift.h"
#include "Components/SphereComponent.h"

ABasePickUpGift::ABasePickUpGift()
{
    PrimaryActorTick.bCanEverTick = true;
    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SetRootComponent(SphereComponent);
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetupAttachment(SphereComponent);
}

void ABasePickUpGift::BeginPlay()
{
    Super::BeginPlay();
}

void ABasePickUpGift::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
