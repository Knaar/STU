#include "Components/WeaponComponent.h"

#include "STUEquipAnimNotify.h"
#include "GameFramework/Character.h"
#include "ShootThemUp/Weapon/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;

    SpawnWeapon();
    EquipWeapon(CurrentWeaponIndex);
    InitAnimations();
    
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

    CurrentWeapon=nullptr;
    for(auto WeaponIndex:Weapons)
    {
        WeaponIndex->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        WeaponIndex->Destroy();
    }
    Weapons.Empty();
    
    Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::SpawnWeapon()
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld())
        return;

    for (auto WeaponIt : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponIt);
        if (!Weapon)
            continue;

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        
        AttachWeaponToMesh(Weapon, Character->GetMesh(), InventoryWeaponSocket);
    }
    
}

void UWeaponComponent::AttachWeaponToMesh(ABaseWeapon *Weapon, USceneComponent *SceneComponent, const FName &SocketName)
{
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    const ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character||!CanChangeWeapon())
        return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire(); 
        AttachWeaponToMesh(CurrentWeapon, Character->GetMesh(), InventoryWeaponSocket);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToMesh(CurrentWeapon, Character->GetMesh(), EquippedWeaponSocket);
    bChangeWeaponInProgress=true;
    SwapWeaponAnimPlay(AnimEquip);
    
}

void UWeaponComponent::NextWeapon()
{
    if(!CanChangeWeapon())return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
          
    EquipWeapon(CurrentWeaponIndex);
   
}

void UWeaponComponent::StartFire()
{
    if (!CanFire())
        return;
    CurrentWeapon->StartFire();
}

void UWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void UWeaponComponent::SwapWeaponAnimPlay(UAnimMontage *PlayAnimMontage)
{
    auto Character=Cast<ACharacter>(GetOwner());
    if(!Character)return;

    Character->PlayAnimMontage(PlayAnimMontage);
    InitAnimations();
}

void UWeaponComponent::InitAnimations()
{
    if(!AnimEquip)return;
    
    auto AllAnimEquipNotifies=AnimEquip->Notifies;
    for (auto SomeOneNotify:AllAnimEquipNotifies)
    {
        auto CorrectNotify=Cast<USTUEquipAnimNotify>(SomeOneNotify.Notify);
        if(CorrectNotify)
        {
            CorrectNotify->OnNotify.AddUObject(this,&UWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* SkeletalMesh)
{
    ACharacter* Character=Cast<ACharacter>(GetOwner());
    if(!Character)return;
    
    if(Character->GetMesh()==SkeletalMesh)
    {
        bChangeWeaponInProgress=false;
    }
}

bool UWeaponComponent::CanFire()
{
    return CurrentWeapon||!bChangeWeaponInProgress;
}

bool UWeaponComponent::CanChangeWeapon()
{
    return !bChangeWeaponInProgress;
}
