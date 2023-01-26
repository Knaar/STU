#include "BasePickUpGift.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"
#include "Sound/SoundCue.h"

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
    UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickUpSound,GetActorLocation());

    
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

USTUHeathComponent * ABasePickUpGift::GetHealthComponent(const APawn *Pawn) const
{
    const auto Player=Pawn;
    if(!Player)return nullptr;

    const auto ComponentH=Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent=Cast<USTUHeathComponent>(ComponentH);
    if(!HealthComponent||HealthComponent->IsDead())return nullptr;
    return HealthComponent;
}

UWeaponComponent * ABasePickUpGift::GetWeaponComponent(const APawn *Pawn) const
{
    const auto Player=Pawn;
    if(!Player)return nullptr;

    const auto ComponentW=Player->GetComponentByClass(UWeaponComponent::StaticClass());
    const auto WeaponComponent=Cast<UWeaponComponent>(ComponentW);
    if(!WeaponComponent) return nullptr;
  
    return WeaponComponent;
}
