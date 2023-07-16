// Copy Right LDW. All Rights Reserved.


#include "System/StateMachine/Player/PlayerHumanBaseState.h"
#include "Characters/Player/CharacterPlayerHuman.h"

UPlayerHumanBaseState::UPlayerHumanBaseState()
{

}

void UPlayerHumanBaseState::InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor)
{
	Super::InitState_Implementation(StateMachineComp, OwnerActor);

	if (ACharacterPlayerHuman* OwnerAsPlayerHuman = Cast<ACharacterPlayerHuman>(OwnerActor))
	{
		OwnerCharacterPlayerHuman = OwnerAsPlayerHuman;
	}
}

void UPlayerHumanBaseState::HandleActionString_Implementation(const FString& ActionID)
{
	Super::HandleActionString_Implementation(ActionID);
	if (!bIsInState) return;

	if (ActionID.Equals(TEXT("end_state")))
	{
		ChangeState(TEXT("player_human_locomotion_state"));
	}
}

bool UPlayerHumanBaseState::CheckValidState_Implementation()
{
	return (Super::CheckValidState_Implementation() && OwnerCharacterPlayerHuman);
}

void UPlayerHumanBaseState::OnInputActionTriggered_Dash_Implementation(bool InputValue)
{
	if (bAutoHandleDashState || (!bAutoHandleDashState && bCanDash))
	{
		ChangeState(TEXT("player_human_dash_state"));
	}
}
