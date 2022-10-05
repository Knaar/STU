// Fill out your copyright notice in the Description page of Project Settings.


#include "STUDevDamagwActor.h"

#include "ColdDamageType.h"
#include "FireDamageType.h"
#include "Kismet/GameplayStatics.h"

ASTUDevDamagwActor::ASTUDevDamagwActor()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent=CreateDefaultSubobject<USceneComponent>("Scene Component");
    SetRootComponent(SceneComponent);
}

void ASTUDevDamagwActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASTUDevDamagwActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Создаём сферу(где, в каком месте, радиус, сегменты и цвет
    DrawDebugSphere(GetWorld(),GetActorLocation(),Radius,24,SphereColor);
    
    UGameplayStatics::ApplyRadialDamage(GetWorld(),DamageDealing,GetActorLocation(),Radius,DamageType,{},this,nullptr,bDoFullDamage);
    
    
   
}
