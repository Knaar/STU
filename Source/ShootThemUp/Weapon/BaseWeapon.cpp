#include "BaseWeapon.h"
#include "GameFramework/Character.h"


DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon  mesh");
    SetRootComponent(WeaponMesh);
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(GetWorld());
}

void ABaseWeapon::Fire()
{
    MakeShot();
    UE_LOG(BaseWeaponLog, Warning, TEXT("Poof"));
}

void ABaseWeapon::MakeShot()
{
    if (!GetWorld())return;

    const auto Player=Cast<ACharacter>(GetOwner());
    if(!Player)return;
    
    const auto Controller=Player->GetController<APlayerController>();
    if(!Controller)return;
    
    FVector CameraLocation;
    FRotator CameraRotation;
    Controller->GetPlayerViewPoint(CameraLocation,CameraRotation);
    
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(SocketName);
    const FVector StartTrace = CameraLocation;//SocketTransform.GetLocation();
    const FVector TraceDirection = CameraRotation.Vector();//SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = StartTrace + TraceDirection * TraceLenght;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult,StartTrace,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(),HitResult.Location,10,10,FColor::Orange,false,3.0f,0,3.0f);
        UE_LOG(BaseWeaponLog, Warning, TEXT("Bone: %s"),*HitResult.BoneName.ToString());
        
        const auto Target= HitResult.GetActor();
        if(!Target)return;
        Target->TakeDamage(BulletDamage,FDamageEvent{},Controller,this);
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
}
