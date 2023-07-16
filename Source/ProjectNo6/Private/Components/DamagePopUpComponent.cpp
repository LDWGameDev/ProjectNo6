// Copy Right LDW. All Rights Reserved.


#include "Components/DamagePopUpComponent.h"

UDamagePopUpComponent::UDamagePopUpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDamagePopUpComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UDamagePopUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

