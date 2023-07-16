// Copy Right LDW. All Rights Reserved.

#include "Components/GameplayTagComponent.h"


UGameplayTagComponent::UGameplayTagComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();

}
