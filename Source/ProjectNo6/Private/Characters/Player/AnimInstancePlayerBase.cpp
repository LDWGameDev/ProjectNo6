// Copy Right LDW. All Rights Reserved.


#include "Characters/Player/AnimInstancePlayerBase.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomFunctionLibrary.h"

void UAnimInstancePlayerBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacterPlayerBase = Cast<ACharacterPlayerBase>(TryGetPawnOwner());
}

void UAnimInstancePlayerBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwnerCharacterPlayerBase) return;
	AnimationIndex = OwnerCharacterPlayerBase->GetFlowIndex();
	bIsInAir = (OwnerCharacterPlayerBase->GetCharacterMovement()) ? (OwnerCharacterPlayerBase->GetCharacterMovement()->IsFalling()) : false;
	MovementSpeed = OwnerCharacterPlayerBase->GetVelocity().Size();
	MovingSpeedAlpha = (OwnerCharacterPlayerBase->DefaultWalkSpeed != 0.0f) ? (MovementSpeed / OwnerCharacterPlayerBase->DefaultWalkSpeed) : 0.0f;
	FVector Velocity = OwnerCharacterPlayerBase->GetVelocity();
	Velocity.Z = 0.0f;
	MovingDirectionAngle = UCustomFunctionLibrary::AngleBetween2Vector(OwnerCharacterPlayerBase->GetActorForwardVector(), Velocity);
}
