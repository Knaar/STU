#include "BasePickUpGift.h"
#include "Components/SphereComponent.h"

ABasePickUpGift::ABasePickUpGift()
{
    PrimaryActorTick.bCanEverTick = true;
    
    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->InitSphereRadius(50.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(SphereComponent);
    
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
    
    const auto Pawn= Cast<APawn>(OtherActor);
    if (GivePickUpTo(Pawn))
    {
        PickupWasTaken();
    }
    
}

bool ABasePickUpGift::GivePickUpTo(APawn *Pawn)
{
    return false;
}

void ABasePickUpGift::PickupWasTaken()
{
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    GetRootComponent()->SetVisibility(false,true);
    //SphereComponent->SetVisibility(false,true);

    
    GetWorldTimerManager().SetTimer(TimerToRestore,this,&ABasePickUpGift::Respawn,RespawnTime);
}

void ABasePickUpGift::Respawn()
{
    MakeRandRotation();
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    GetRootComponent()->SetVisibility(true,true);
    //SphereComponent->SetVisibility(true,true);
}

void ABasePickUpGift::MakeRandRotation()
{
    bool Rand=FMath::RandBool();
    int32 NewInt=Rand? 1:-1;

    RotationValue=FMath::RandRange(0.1f,2.0f)*NewInt;
}

bool ABasePickUpGift::CouldBeTaken()const
{
    return !GetWorldTimerManager().IsTimerActive(TimerToRestore);
}
