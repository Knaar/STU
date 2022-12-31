// Fill out your copyright notice in the Description page of Project Settings.


#include "STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShootThemUp/Components/WeaponComponent.h"


ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm-> SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation=true;
    SpringArm->SocketOffset=FVector(0.0f,100.0f,80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArm);

    CameraCollisionComponent=CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
    
}

void ASTUPlayerCharacter::OnPlayerDeath()
{
    Super::OnPlayerDeath();
    if(Controller)
    {
        Controller->ChangeState(NAME_Spectating); 
    }
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
        PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);
        PlayerInputComponent->BindAction("Jump",EInputEvent::IE_Pressed, this, &ASTUPlayerCharacter::Jump);
        PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
        PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);
    
        PlayerInputComponent->BindAction("Fire",IE_Pressed,WeaponComponent,&UWeaponComponent::StartFire);
        PlayerInputComponent->BindAction("Fire",IE_Released,WeaponComponent,&UWeaponComponent::StopFire);
        
        PlayerInputComponent->BindAction("Swap",IE_Released,WeaponComponent,&UWeaponComponent::NextWeapon);
        PlayerInputComponent->BindAction("Reload",IE_Released,WeaponComponent,&UWeaponComponent::Reload);
}

void ASTUPlayerCharacter::OnStartRunning()
{
    bWantsToRun=true;
    GetCharacterMovement()->MaxWalkSpeed = 800;
}

void ASTUPlayerCharacter::OnStopRunning()
{
    bWantsToRun=false;
    GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto bHideMesh=CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(bHideMesh);

    TArray<USceneComponent*>MeshChildren;
    GetMesh()->GetChildrenComponents(true,MeshChildren);

    for(auto MeshChild:MeshChildren)
    {
        const auto MeshChildGeometry=Cast<UPrimitiveComponent>(MeshChild);
        if(MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(bHideMesh);
        }
    }
}

