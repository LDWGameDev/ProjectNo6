// Copy Right LDW. All Rights Reserved.

#include "System/StateMachine/Player/PlayerHumanDashState.h"
#include "Characters/Player/CharacterPlayerHuman.h"
#include "Components/AnimationComponent.h"
#include "System/Settings/CustomGameUserSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TransformComponent.h"

UPlayerHumanDashState::UPlayerHumanDashState()
{
	StateID = TEXT("player_human_dash_state");
}

void UPlayerHumanDashState::EnterState_Implementation()
{
	Super::EnterState_Implementation();
	if (!OwnerCharacterPlayerHuman) return;
	OwnerCharacterPlayerHuman->CustomPlaySound(TEXT("player_short_effort_groan"), FVector());

	OwnerCharacterPlayerHuman->bShouldRotateToMouse = false;
	if (OwnerCharacterPlayerHuman->GetAnimationComponent())
	{
		OwnerCharacterPlayerHuman->GetAnimationComponent()->PlayCustomMontage(TEXT("dash"), FName(TEXT("Default")), 1.0f);
	}

	if (UCustomGameUserSettings::Get() && UCustomGameUserSettings::Get()->GetDashToCursor() && OwnerCharacterPlayerHuman->GetTransformComponent())
	{
		FRotator LookAtMouseRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacterPlayerHuman->GetActorLocation(), OwnerCharacterPlayerHuman->GetGameplayMouseLocation());
		LookAtMouseRotation.Pitch = 0.0f;
		LookAtMouseRotation.Roll = 0.0f;
		OwnerCharacterPlayerHuman->GetTransformComponent()->SetOwnerActorRotation(LookAtMouseRotation, true);
	}
	else if (!OwnerCharacterPlayerHuman->LastMovementInput.IsNearlyZero(0.1f) && OwnerCharacterPlayerHuman->GetTransformComponent())
	{
		FVector TargetDirection = OwnerCharacterPlayerHuman->LastMovementInput.Y * FVector::ForwardVector + OwnerCharacterPlayerHuman->LastMovementInput.X * FVector::RightVector;
		TargetDirection.Z = 0;
		TargetDirection.Normalize();
		FRotator TargetRotation = FRotationMatrix::MakeFromXZ(TargetDirection, FVector::UpVector).Rotator();
		OwnerCharacterPlayerHuman->GetTransformComponent()->SetOwnerActorRotation(TargetRotation, true);
	}
}

void UPlayerHumanDashState::TickState_Implementation(float DeltaTime)
{
	Super::TickState_Implementation(DeltaTime);

	//if (bActionCanBreak)
	//{
	//	if (bSaveLightAttackInput)
	//	{
	//		ChangeState(TEXT("player_human_gauntlet_lad_state"));
	//	}
	//}
}

void UPlayerHumanDashState::ExitState_Implementation()
{
	Super::ExitState_Implementation();
	OwnerCharacterPlayerHuman->bShouldRotateToMouse = true;
}