#include "STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    SetRootComponent(SphereCollision);
    SphereCollision->InitSphereRadius(5.0f);
    SphereCollision->IgnoreActorWhenMoving(GetOwner(),true);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed=2000.0f;
    MovementComponent->ProjectileGravityScale=0.0f;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    MovementComponent->Velocity=ProjectileDirection*MovementComponent->InitialSpeed;
    SetLifeSpan(ProjectileLifeTime);
    SphereCollision->OnComponentHit.AddDynamic(this,&ASTUProjectile::ComponentHit);
}

void ASTUProjectile::ComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld())return;
    MovementComponent->StopMovementImmediately();
    
    DrawDebugSphere(GetWorld(),GetActorLocation(),BlowSphereRadius,20,FColor::Blue,false,1.0f,0.0,3.0);

    
    UGameplayStatics::ApplyRadialDamage(GetWorld(),BlowSphereDamage,GetActorLocation(),BlowSphereRadius,UDamageType::StaticClass(),{GetOwner()},this,GetController(),bDoFullDamage);
    Destroy();
}

AController *ASTUProjectile::GetController()
{
    const auto Player=Cast<APawn>(GetOwner());
    return Player ? Player->GetController():nullptr;
}
