// Copy Right LDW. All Rights Reserved.


#include "Actors/TemplateBase.h"
#include "GameFramework/PlayerStart.h"

ATemplateBase::ATemplateBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);

	PlayerStart = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerStart"));
	PlayerStart->SetupAttachment(RootSceneComp);
	PlayerStart->SetChildActorClass(APlayerStart::StaticClass());

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(RootSceneComp);
}

void ATemplateBase::BeginPlay()
{
	Super::BeginPlay();
	
}

