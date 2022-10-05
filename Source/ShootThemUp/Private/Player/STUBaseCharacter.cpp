// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/STUHeathComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{

    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm-> SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation=true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArm);

    HealthComponent=CreateDefaultSubobject<USTUHeathComponent>("HealthComponent");

    TextRenderComponent=CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    TextRenderComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    //���� ��������
    check(HealthComponent);
    check(TextRenderComponent);
    check(AnimMontage);
    
    //����� ��������� ������ ���������� ��������
    float Health=HealthComponent->GetHealth();
    OnHealthChanged(Health);

    //������� ���������
    HealthComponent->OnPlayerDamaged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    HealthComponent->OnPlayerDeath.AddUObject(this,&ThisClass::OnPlayerDeath);

    LandedDelegate.AddDynamic(this,&ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //���������� UE �-�. ���:(������ ������,������������� ������,����������(���� ��������� ���� ��� ����),��� ��������
    //TakeDamage(0.1f,FDamageEvent{},Controller,this);
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

/*��� ������ ���� ������� ������ �����. ��, AddControllerPithInput � AddControllerYawInput ����� ��������� ��������
void ASTUBaseCharacter::LookUp(float Amount)
{
    AddControllerPitchInput(Amount);
}

void ASTUBaseCharacter::TurnAround(float Amount)
{
    AddControllerYawInput(Amount);
}*/
void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    //����� �������� ������ ��������������������
    TextRenderComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"),Health)));
}

inline void ASTUBaseCharacter::OnPlayerDeath()
{
    //����������� ������, ��������� ����� � ������� ���������
    PlayAnimMontage(AnimMontage);
    GetCharacterMovement()->Deactivate();
    SetLifeSpan(2);

    if(Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    auto VectorVelocityZ=-GetCharacterMovement()->Velocity.Z;
    UE_LOG(LogTemp,Warning,TEXT("GetCharacterMovementVolocity : %f"),VectorVelocityZ);
    
    if (VectorVelocityZ<VectorDamageVelocity.X){return;}

    const auto TotalDamage=FMath::GetMappedRangeValueClamped(VectorDamageVelocity,VectorDamage,VectorVelocityZ);
    TakeDamage(TotalDamage,FDamageEvent{},nullptr,nullptr);
    UE_LOG(LogTemp,Warning,TEXT("Total Damade : %f"),TotalDamage);
}
