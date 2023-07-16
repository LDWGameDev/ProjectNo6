// Copy Right LDW. All Rights Reserved.


#include "System/Equipment/HoldingActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AHoldingActor::AHoldingActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);
	
	HoldingMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoldingMeshComp"));
	HoldingMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoldingMeshComp->SetupAttachment(RootSceneComp);
}

void AHoldingActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHoldingActor::OnStartHolding_Implementation()
{

}

void AHoldingActor::OnStopHolding_Implementation()
{

}