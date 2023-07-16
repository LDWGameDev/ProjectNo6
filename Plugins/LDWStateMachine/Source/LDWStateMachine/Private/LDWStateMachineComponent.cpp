// Copy Right LDW. All Rights Reserved.


#include "LDWStateMachineComponent.h"
#include "LDWBaseState.h"

ULDWStateMachineComponent::ULDWStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bIsActivated = false;
	StateHistoryNumber = 20;
}

void ULDWStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	StateHistoryList.Reserve(StateHistoryNumber);
	for (ULDWBaseState* TempState : StateList)
	{
		if (TempState) TempState->InitState(this, GetOwner());
	}
}

void ULDWStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsActivated && CurrentState)
	{
		CurrentState->TickState(DeltaTime);
	}
}

void ULDWStateMachineComponent::ActivateStateMachine(const FString& InitialStateID)
{
	if (bIsActivated) return;
	if (ULDWBaseState* InitialState = FindState(InitialStateID))
	{
		if (CurrentState) CurrentState->ExitState();
		InitialState->EnterState();
		CurrentState = InitialState;

		bIsActivated = true;
		SetComponentTickEnabled(true);
	}
}

void ULDWStateMachineComponent::DeactivateStateMachine()
{
	if (CurrentState)
	{
		CurrentState->ExitState();
		CurrentState = nullptr;

		bIsActivated = false;
		SetComponentTickEnabled(false);
	}
}

void ULDWStateMachineComponent::ChangeState(const FString& NewStateID)
{
	if (NewStateID.IsEmpty()) return;
	if (!bIsActivated)
	{
		// Activate StateMachine with NewStateID if have not
		ActivateStateMachine(NewStateID);
	}
	else
	{
		bool SuccessfullyChangedToNewState = true;
		if (ULDWBaseState* NewState = FindState(NewStateID))
		{
			// Change to new state with NewStateID
			if (CurrentState) CurrentState->ExitState();
			NewState->EnterState();
			CurrentState = NewState;
		}
		else if (ULDWBaseState* DefaultState = FindState(DefaultStateID))
		{
			// Change to default state with DefaultStateID
			if (CurrentState) CurrentState->ExitState();
			DefaultState->EnterState();
			CurrentState = DefaultState;
		}
		else
		{
			// State with id NewStateID and DefaultStateID do not exist
			SuccessfullyChangedToNewState = false;
		}

		// Add new state to state history list
		if (SuccessfullyChangedToNewState)
		{
			if (StateHistoryList.Num() >= StateHistoryNumber)
			{
				StateHistoryList.RemoveAt(StateHistoryNumber - 1, 1, false);
			}
			StateHistoryList.Insert(CurrentState, 0);
		}
	}
}

ULDWBaseState* ULDWStateMachineComponent::FindState(const FString& StateID)
{
	if (!StateID.IsEmpty() && StateList.Num() != 0)
	{
		for (ULDWBaseState* TempState : StateList)
		{
			if (TempState && TempState->GetStateID().Equals(StateID))
			{
				return TempState;
			}
		}
	}
	return nullptr;
}

