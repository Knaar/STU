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
    MakeRandRotation();
}

void ABasePickUpGift::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.0f,RotationValue,0.0f));
}

void ABasePickUpGift::NotifyActorBeginOverlap(AActor *OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (IsGiftPickedUp(OtherActor))
    {
        PickupGift();
    }
}

bool ABasePickUpGift::IsGiftPickedUp(AActor *Actor)
{
    RestoreGift();
    return false;
}

void ABasePickUpGift::PickupGift()
{
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetVisibility(false,true);

    FTimerHandle TimerToRestore;
    GetWorldTimerManager().SetTimer(TimerToRestore,this,&ABasePickUpGift::RestoreGift,5.0f);
}

void ABasePickUpGift::RestoreGift()
{
    MakeRandRotation();
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    SphereComponent->SetVisibility(true,true);
}

void ABasePickUpGift::MakeRandRotation()
{
    bool Rand=FMath::RandBool();
    int32 NewInt=Rand? 1:-1;

    RotationValue=FMath::RandRange(0.1f,2.0f)*NewInt;
}
