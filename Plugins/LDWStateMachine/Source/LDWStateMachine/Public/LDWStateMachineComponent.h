// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LDWStateMachineComponent.generated.h"

class ULDWBaseState;

/**
 * Component handles states of a character
 * Only 1 state can be active at 1 time
 * Should be added to character
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LDWSTATEMACHINE_API ULDWStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Machine")
		FString DefaultStateID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State Machine")
		int32 StateHistoryNumber;
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "State Machine : States")
		TArray<ULDWBaseState*> StateList;

	UPROPERTY()
		ULDWBaseState* CurrentState;
	UPROPERTY()
		TArray<ULDWBaseState*> StateHistoryList;

	bool bIsActivated;

public:	
	ULDWStateMachineComponent();

	/** UActorComponent override functions */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	/** Getters, setters */
	const TArray<ULDWBaseState*>& GetStateList() { return StateList; }
	const TArray<ULDWBaseState*>& GetStateHistoryList() { return StateHistoryList; }
	ULDWBaseState* GetCurrentState() { return CurrentState; }
	bool CheckIsActivated() { return bIsActivated; }
	
	/** Get current state as T type */
	template <class T>
	T* GetCurrentState() { return Cast<T>(CurrentState); }

	UFUNCTION(BlueprintCallable, Category = "State Machine")
		ULDWBaseState* K2_GetCurrentState() { return CurrentState; }

	/** Activate function. Fail if not find state or has been activated */
	virtual void ActivateStateMachine(const FString& InitialStateID);
	/** Deactivate function */
	virtual void DeactivateStateMachine();

	/** Changes to change to new state with NewStateID. If is not activated, executes Activate() function instead. If do not find state with NewStateID, Change to default state instead */
	virtual void ChangeState(const FString& NewStateID);

	/** Finds state with StateID from StateList */
	ULDWBaseState* FindState(const FString& StateID);

	/** Finds state with StateID from StateList and cast to T type*/
	template <class T>
	FORCEINLINE T* FindState(const FString& StateID)
	{
		if (!StateID.IsEmpty() && StateList.Num() != 0)
		{
			for (ULDWBaseState* TempState : StateList)
			{
				if (TempState->GetStateID().Equals(StateID))
				{
					return Cast<T>(TempState);
				}
			}
		}
		return nullptr;
	}

protected:
	/** UActorComponent override functions */
	virtual void BeginPlay() override;
};
