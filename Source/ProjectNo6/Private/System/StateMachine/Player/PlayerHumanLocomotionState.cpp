// Copy Right LDW. All Rights Reserved.


#include "System/StateMachine/Player/PlayerHumanLocomotionState.h"
#include "Characters/Player/CharacterPlayerHuman.h"
#include "System/StateMachine/Player/PlayerHumanBaseState.h"
#include "System/StateMachine/Player/PlayerStateMachineComponent.h"
#include "System/Equipment/EquipmentComponent.h"


UPlayerHumanLocomotionState::UPlayerHumanLocomotionState()
{
	StateID = TEXT("player_human_locomotion_state");
}

void UPlayerHumanLocomotionState::EnterState_Implementation()
{
	Super::EnterState_Implementation();
	if(!OwnerCharacterPlayerHuman) return;
	if (OwnerCharacterPlayerHuman->GetEquipmentComp())
	{
		OwnerCharacterPlayerHuman->GetEquipmentComp()->bCanChangeEquipmentIndex = true;
	}

	// TODO: check for holding light attack input
	if (OwnerCharacterPlayerHuman->bIsHoldingLightAttackInput)
	{
		
	}
	else if (OwnerCharacterPlayerHuman->bIsHoldingHeavyAttackInput)
	{
		
	}
}

void UPlayerHumanLocomotionState::TickState_Implementation(float DeltaTime)
{
	Super::TickState_Implementation(DeltaTime);
}

void UPlayerHumanLocomotionState::ExitState_Implementation()
{
	Super::ExitState_Implementation();
	if (!OwnerCharacterPlayerHuman) return;
	if (OwnerCharacterPlayerHuman->GetEquipmentComp())
	{
		OwnerCharacterPlayerHuman->GetEquipmentComp()->bCanChangeEquipmentIndex = false;
	}
}

void UPlayerHumanLocomotionState::OnInputActionTriggered_Movement_Implementation(const FVector2D& InputValue)
{
	if (OwnerCharacterPlayerHuman)
	{
		OwnerCharacterPlayerHuman->MoveWithInput(InputValue);
	}
}

void UPlayerHumanLocomotionState::OnInputActionTriggered_Dash_Implementation(bool InputValue)
{
	if (!OwnerCharacterPlayerHuman) return;
	ChangeState(TEXT("player_human_dash_state"));
}

void UPlayerHumanLocomotionState::OnInputAction_LightAttack_Started_Implementation()
{
	if (!OwnerCharacterPlayerHuman || !OwnerCharacterPlayerHuman->GetEquipmentComp()) return;
	switch (OwnerCharacterPlayerHuman->GetEquipmentComp()->GetCurrentHoldingType())
	{
	case EHoldingObjectType::Torch:
	{
		ChangeState(TEXT("player_human_torch_la"));
		break;
	}
	case EHoldingObjectType::GunSword:
	{
		ChangeState(TEXT("player_human_gunsword_la_0"));
		break;
	}
	}
}

void UPlayerHumanLocomotionState::OnInputAction_LightAttack_Canceled_Implementation()
{
	
}

void UPlayerHumanLocomotionState::OnInputAction_LightAttack_Completed_Implementation()
{
	
}

void UPlayerHumanLocomotionState::OnInputAction_HeavyAttack_Started_Implementation()
{
	if (!OwnerCharacterPlayerHuman || !OwnerCharacterPlayerHuman->GetEquipmentComp()) return;
	switch (OwnerCharacterPlayerHuman->GetEquipmentComp()->GetCurrentHoldingType())
	{
	case EHoldingObjectType::GunSword:
	{
		ChangeState(TEXT("player_human_gunsword_ha"));
		break;
	}
	}
}
