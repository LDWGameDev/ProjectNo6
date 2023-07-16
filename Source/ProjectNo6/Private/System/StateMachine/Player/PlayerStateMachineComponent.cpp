// Copy Right LDW. All Rights Reserved.


#include "System/StateMachine/Player/PlayerStateMachineComponent.h"

UPlayerStateMachineComponent::UPlayerStateMachineComponent()
{

}

UPlayerBaseState* UPlayerStateMachineComponent::GetState(FString StateID)
{
	return FindState<UPlayerBaseState>(StateID);
}
