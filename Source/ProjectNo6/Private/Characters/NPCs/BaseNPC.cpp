// Copy Right LDW. All Rights Reserved.


#include "Characters/NPCs/BaseNPC.h"

ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

