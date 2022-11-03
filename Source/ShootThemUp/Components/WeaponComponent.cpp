#include "WeaponComponent.h"

#include "AnimUtils.h"
#include "STUEquipAnimNotify.h"
#include "ShootThemUp/Weapon/BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

constexpr static int32 WeaponsNum=2;

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;

    checkf(WeaponData.Num()==WeaponsNum, TEXT("Character can hold only 2 weapon if items"));
    
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
        Weapon->OnReloadEmptyClip.AddUObject(this,&UWeaponComponent::ObReloadEmptyClip);

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
    const auto EqiupNotify = AnimUtils::FindAnimNotifies<USTUEquipAnimNotify>(CurrentAnimEquipMontage);
    if (EqiupNotify)
        {
            EqiupNotify->OnNotify.AddUObject(this, &UWeaponComponent::OnEquipFinished);
        }
    else
    {
        UE_LOG(LogTemp,Error,TEXT("Equip anim notify is forgotten to set"))
        checkNoEntry();
    }

    for (auto CurrentWeaponDataRunner:WeaponData)
    {
        const auto ReloadNotify=AnimUtils::FindAnimNotifies<UBaseAnimNotify>(CurrentWeaponDataRunner.DataAnimMontage);
        if(ReloadNotify)
        {
            ReloadNotify->OnNotify.AddUObject(this,&UWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(LogTemp,Error,TEXT("Reload anim notify is forgotten to set"))
            checkNoEntry();
        }
    }
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
    return CurrentWeapon && !bChangeWeaponInProgress && !bReloadingInProgress&&CurrentWeapon->bCanReload();
}

void UWeaponComponent::ObReloadEmptyClip(ABaseWeapon* AmmoEmptyWeapon)
{
    if(CurrentWeapon==AmmoEmptyWeapon)
    {
        ChangeClip();  
    }
    else
    {
        for(auto WeaponsRunner:Weapons)
        {
            if(WeaponsRunner==AmmoEmptyWeapon)
            {
                WeaponsRunner->ChangeClip();
            }
        }
    }
    
}


void UWeaponComponent::Reload()
{
        ChangeClip();
}

void UWeaponComponent::ChangeClip()
{
    if(!CanReload())return;
    bReloadingInProgress = true;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    PlayAnimation(CurrentReloadAnimMontage);
}

bool UWeaponComponent::GetMyWeaponImageData(FMyWeaponImageData& ImgData) const
{
    if(CurrentWeapon)
    {
        ImgData=CurrentWeapon->GetMyWeaponImageData();
        return true;
    }
    return false;
}

bool UWeaponComponent::GetMyWeaponAmmo(FMyAmmo &AmmoData) const
{
    if(CurrentWeapon)
    {
        AmmoData=CurrentWeapon->GetMyWeaponAmmoData();
        return true;
    }
    return false;
}

bool UWeaponComponent::TryToAddBullets(int32 AddedAmmo, TSubclassOf<ABaseWeapon>Weapon)
{
    UE_LOG(WeaponComponentLog,Warning,TEXT("Try to add bullets in weaponComponent"))
    //if(!CurrentWeapon)return false;
    for(auto WeaponRunner:Weapons)
    {
        if(WeaponRunner&&WeaponRunner->IsA(Weapon))
        {
            WeaponRunner->TryToAddAmmo(AddedAmmo);
            return true;
        }
    }
    return false;
}
