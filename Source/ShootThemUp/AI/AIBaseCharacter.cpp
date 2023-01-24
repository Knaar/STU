// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBaseCharacter.h"
#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShootThemUp/Components/STUHeathComponent.h"
#include "ShootThemUp/UI/STUHealthBarWidget.h"

AAIBaseCharacter::AAIBaseCharacter(const FObjectInitializer& ObjInit)
    :Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComp"))
{
    AutoPossessAI=EAutoPossessAI::Disabled;
    AIControllerClass=ASTUAIController::StaticClass();

    bUseControllerRotationYaw=false;
    if(GetCharacterMovement())
    {
        GetCharacterMovement()->RotationRate=FRotator(0.0f,250.0f,0.0f);
        GetCharacterMovement()->bUseControllerDesiredRotation=true;
    }
    HealthWidgetComponent=CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
    
}

void AAIBaseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateHealthWidgetVisibility();
    
}

void AAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthWidgetComponent);
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if(HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
        UE_LOG(LogTemp,Warning,TEXT("%f"),HealthComponent->GetHealthPercent());
        
    }
}

void AAIBaseCharacter::OnPlayerDeath()
{
    Super::OnPlayerDeath();
    const auto STUController=Cast<AAIController>(Controller);
    if(STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}

void AAIBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
    Super::OnHealthChange(Health, HealthDelta);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if(HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
        UE_LOG(LogTemp,Warning,TEXT("%f"),HealthComponent->GetHealthPercent());
        
    }
}

void AAIBaseCharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() ||                                      //
        !GetWorld()->GetFirstPlayerController() ||           //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())  //
            return;

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}

