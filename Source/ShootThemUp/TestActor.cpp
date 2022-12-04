#include "TestActor.h"
#include "Components\StaticMeshComponent.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComponent;
    
    BodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(RootComponent);
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

    
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

