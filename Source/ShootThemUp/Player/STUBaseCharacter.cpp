// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"
#include "Sound/SoundCue.h"
// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) 
   // :Super(ObjInit.SetDefaultSubobjectClass<USTUC>())
{
    PrimaryActorTick.bCanEverTick = true;
    
    HealthComponent=CreateDefaultSubobject<USTUHeathComponent>("HealthComponent");
    WeaponComponent=CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    check(HealthComponent);
    check(AnimMontage);
    check(GetMesh());
    
    float Health=HealthComponent->GetHealth();
    
    HealthComponent->OnPlayerDeath.AddUObject(this,&ThisClass::OnPlayerDeath);
    HealthComponent->OnPlayerDamaged.AddUObject(this, &ASTUBaseCharacter::OnHealthChange);
    
    LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);
    OnButtonPressed.Broadcast();
    
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
}
void ASTUBaseCharacter::OnPlayerDeath()
{
    //PlayAnimMontage(AnimMontage);
    GetCharacterMovement()->Deactivate();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComponent->StopFire();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetSimulatePhysics(true);

    if(DeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
    }
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount*2);
    FVector Vel = GetVelocity();
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(),Amount);
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto VectorVelocityZ=-GetCharacterMovement()->Velocity.Z;
    UE_LOG(LogTemp,Warning,TEXT("GetCharacterMovementVolocity : %f"),VectorVelocityZ);
    
    if (VectorVelocityZ<LandedDamageVelocity.X){return;}

    const auto TotalDamage=FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage,VectorVelocityZ);
    TakeDamage(TotalDamage,FDamageEvent{},nullptr,nullptr);
    UE_LOG(LogTemp,Warning,TEXT("Total Damade : %f"),TotalDamage);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor &Color) const
{
    //нужно создать динамический материал из материала, который находится на меше
    const auto MaterialInst=GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if(!MaterialInst)return;

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
    
}

