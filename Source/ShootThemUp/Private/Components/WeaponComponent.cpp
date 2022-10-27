#include "Components/WeaponComponent.h"
#include "STUEquipAnimNotify.h"
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
    CurrentWeapon = nullptr;
    for (auto WeaponIndex : Weapons)
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

    for (auto OnWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OnWeaponData.DataWeapon);
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
    if (!Character || !CanChangeWeapon() || WeaponIndex < 0 || WeaponIndex >= Weapons.Num())return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToMesh(CurrentWeapon, Character->GetMesh(), InventoryWeaponSocket);
    }

    CurrentWeapon = Weapons[WeaponIndex];

    const auto TempWeaponData = WeaponData.FindByPredicate([&](const FMyWeaponData &Data)
    {
        return Data.DataWeapon == CurrentWeapon->GetClass();
    });
    CurrentReloadAnimMontage = TempWeaponData ? TempWeaponData->DataAnimMontage : nullptr;

    AttachWeaponToMesh(CurrentWeapon, Character->GetMesh(), EquippedWeaponSocket);
    bChangeWeaponInProgress = true;
    PlayAnimation(CurrentAnimEquipMontage);
}

void UWeaponComponent::NextWeapon()
{
    if (!CanChangeWeapon())
        return;
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

void UWeaponComponent::PlayAnimation(UAnimMontage *PlayAnimMontage)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;
    
    Character->PlayAnimMontage(PlayAnimMontage);
}

void UWeaponComponent::InitAnimations()
{
    const auto EqiupNotify = FindAnimNotifies<USTUEquipAnimNotify>(CurrentAnimEquipMontage);
    if (EqiupNotify)
        {
            EqiupNotify->OnNotify.AddUObject(this, &UWeaponComponent::OnEquipFinished);
        }

    for (auto CurrentWeaponDataRunner:WeaponData)
    {
        const auto ReloadNotify=FindAnimNotifies<UBaseAnimNotify>(CurrentWeaponDataRunner.DataAnimMontage);
        if(ReloadNotify)
        {
            ReloadNotify->OnNotify.AddUObject(this,&UWeaponComponent::OnReloadFinished);
        }
    }
}

template <typename T> T * UWeaponComponent::FindAnimNotifies(UAnimSequenceBase *Animation)
{
    if (!Animation) return nullptr;
    
    const auto AnimEquipNotifyAll=Animation->Notifies;
    for(auto CurrentNEquipNotify:AnimEquipNotifyAll)
    {
        auto Notify=Cast<T>(CurrentNEquipNotify.Notify);
        if(!Notify)continue;
        return Notify;
    }
    return nullptr;
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent *SkeletalMesh)
{
    const ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    UE_LOG(WeaponComponentLog,Warning,TEXT("Equipped"))
    if (Character->GetMesh() == SkeletalMesh)
    {
        bChangeWeaponInProgress = false;
    }
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent *SkeletalMesh)
{
    const ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    UE_LOG(WeaponComponentLog,Warning,TEXT("Reloaded"))
    if (Character->GetMesh() == SkeletalMesh)
    {
        bReloadingInProgress = false;
    }
}

bool UWeaponComponent::CanFire() 
{
    return CurrentWeapon && !bChangeWeaponInProgress && !bReloadingInProgress;
}

bool UWeaponComponent::CanChangeWeapon()
{
    return !bChangeWeaponInProgress && !bReloadingInProgress;
}

bool UWeaponComponent::CanReload()
{
    return CurrentWeapon && !bChangeWeaponInProgress && !bReloadingInProgress;
}


void UWeaponComponent::Reload()
{
    if(!CanReload())return;
    bReloadingInProgress = true;
    PlayAnimation(CurrentReloadAnimMontage);
}
