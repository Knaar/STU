#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "ShootThemUp/Weapon/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponent,All,All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    AttachWeapon();
}

void UWeaponComponent::AttachWeapon()
{
    if (!GetWorld())return;

    ACharacter* Character=Cast<ACharacter>(GetOwner());
    if(!Character)return;

    CurrentWeapon=GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
    if(!CurrentWeapon)return;

    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(),AttachmentRules,SocketName);
    CurrentWeapon->SetOwner(Character);
}

void UWeaponComponent::Fire()
{
    UE_LOG(WeaponComponent,Warning,TEXT("WeaponComponent Fire"));
    if(!CurrentWeapon)return;
    CurrentWeapon->Fire();
}
