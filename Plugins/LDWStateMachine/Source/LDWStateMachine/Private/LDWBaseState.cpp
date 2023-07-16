// Copy Right LDW. All Rights Reserved.

#include "LDWBaseState.h"
#include "Misc/DateTime.h"
#include "LDWStateMachineComponent.h"


ULDWBaseState::ULDWBaseState()
{
	bIsInState = false;
}

void ULDWBaseState::InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor)
{
	StateHistoryList.Reserve(10);
	OwnerStateMachineComp = StateMachineComp;
	EnteredStateTime = FDateTime::UtcNow();
	ExitedStateTime = FDateTime::UtcNow();
}

void ULDWBaseState::EnterState_Implementation()
{
	bIsInState = true;
	EnteredStateTime = FDateTime::UtcNow();
}

void ULDWBaseState::TickState_Implementation(float DeltaTime)
{
	
}

void ULDWBaseState::ExitState_Implementation()
{
	bIsInState = false;
	ExitedStateTime = FDateTime::UtcNow();

	// Add to state history list
	if (StateHistoryList.Num() >= 10)
	{
		StateHistoryList.RemoveAt(9, 1, false);
	}
	StateHistoryList.Insert(FStateHistory(EnteredStateTime, ExitedStateTime), 0);
}

float ULDWBaseState::GetStateTime_LastEnteredToNow()
{
	// return (FDateTime::UtcNow() - EnteredStateTime).GetTotalSeconds();
	return (StateHistoryList.Num() == 0) ? 100.0f : (FDateTime::UtcNow() - EnteredStateTime).GetTotalSeconds();
}

float ULDWBaseState::GetStateTime_LastExitedToNow()
{
	// return ((FDateTime::UtcNow() - ExitedStateTime).GetTotalSeconds());
	return (StateHistoryList.Num() == 0) ? 100.0f : (FDateTime::UtcNow() - ExitedStateTime).GetTotalSeconds();
}

float ULDWBaseState::GetStateTime_LastDuration()
{
	return (StateHistoryList.Num() == 0) ? -1.0f : (StateHistoryList[0].GetDuration());
}

void ULDWBaseState::HandleActionString_Implementation(const FString& ActionID)
{
	
}

void ULDWBaseState::SetLastExitedTimeToNow_Implementation()
{
	ExitedStateTime = FDateTime::UtcNow();
}

void ULDWBaseState::SetLastEnteredTimeToNow_Implementation()
{
	EnteredStateTime = FDateTime::UtcNow();
}

void ULDWBaseState::ChangeState_Implementation(const FString& NewStateID)
{
	if (OwnerStateMachineComp)
	{
		OwnerStateMachineComp->ChangeState(NewStateID);
	}
}
