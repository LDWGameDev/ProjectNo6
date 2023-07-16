// Copy Right LDW. All Rights Reserved.


#include "Characters/Player/AnimInstancePlayerHuman.h"
#include "Characters/Player/CharacterPlayerHuman.h"

void UAnimInstancePlayerHuman::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerPlayerHuman = Cast<ACharacterPlayerHuman>(TryGetPawnOwner());
}

void UAnimInstancePlayerHuman::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerPlayerHuman) return;
}
