// Fill out your copyright notice in the Description page of Project Settings.

#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"


// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
   // :Super(ObjInit.SetDefaultSubobjectClass<USTUC>())
{

   
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm-> SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation=true;
    SpringArm->SocketOffset=FVector(0.0f,100.0f,80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArm);

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
    OnHealthChanged(Health,0.0f);

    
    HealthComponent->OnPlayerDamaged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    HealthComponent->OnPlayerDeath.AddUObject(this,&ThisClass::OnPlayerDeath);

    LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);

    OnButtonPressed.Broadcast();
    
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASTUBaseCharacter::OnStopRuning);

    PlayerInputComponent->BindAction("Fire",IE_Pressed,WeaponComponent,&UWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire",IE_Released,WeaponComponent,&UWeaponComponent::StopFire);
    
    PlayerInputComponent->BindAction("Swap",IE_Released,WeaponComponent,&UWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload",IE_Released,WeaponComponent,&UWeaponComponent::Reload);
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


void ASTUBaseCharacter::OnStartRunning()
{
    bWantsToRun=true;
    GetCharacterMovement()->MaxWalkSpeed = 800;
}

void ASTUBaseCharacter::OnStopRuning()
{
    bWantsToRun=false;
    GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ASTUBaseCharacter::OnHealthChanged(float Health,float DeltaHealth)
{
    
}

inline void ASTUBaseCharacter::OnPlayerDeath()
{
    
    //PlayAnimMontage(AnimMontage);
    GetCharacterMovement()->Deactivate();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    if(Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    WeaponComponent->StopFire();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetSimulatePhysics(true);
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
/*
void ASTUBaseCharacter::SpawnWeapon()
{
    if (!GetWorld())return;

    const auto MyWeapon=GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);

    if (MyWeapon)
    {
        ACharacter* Character=Cast<ACharacter>(GetOwner());
        FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget,false);

        MyWeapon->AttachToComponent(GetMesh(),AttachmentTransformRules,"WeaponSocket");
        //GetMesh()->AttachToComponent(GetWorld(),AttachmentTransformRules,"WeaponSocket");
    }
}
*/
