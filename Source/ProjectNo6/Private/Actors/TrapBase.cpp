// Copy Right LDW. All Rights Reserved.


#include "Actors/TrapBase.h"
#include "Components/SphereComponent.h"
#include "System/Combat/CombatComponent.h"
#include "System/Stat/StatComponent.h"

ATrapBase::ATrapBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	//SetRootComponent(RootSceneComp);

	//CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	//StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
}

void ATrapBase::BeginPlay()
{
	Super::BeginPlay();

}

bool ATrapBase::ActivateTrap_Implementation()
{
	if (bIsActivated || bIsInCooldown) return false;
	bIsActivated = true;
	return true;
}

void ATrapBase::DeactivateTrap_Implementation()
{
	bIsActivated = false;
	bIsInCooldown = true;
	FTimerHandle CooldownTimerHandle;
	FTimerDelegate CooldownTimerDelegate;
	CooldownTimerDelegate.BindLambda([&](){
		bIsInCooldown = false;
	});
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, CooldownTimerDelegate, CooldownTime, false);
}
