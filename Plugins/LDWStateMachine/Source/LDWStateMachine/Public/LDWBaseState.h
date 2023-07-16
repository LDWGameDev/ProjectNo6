// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LDWStateMachineData.h"
#include "GameplayTagContainer.h"
#include "LDWBaseState.generated.h"

class ULDWStateMachineComponent;

/**
 * 
 */
UCLASS(NotBlueprintType)
class LDWSTATEMACHINE_API ULDWBaseState : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LDW Base State")
		FString StateID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LDW Base State")
		FGameplayTagContainer StateTagContainer;
	UPROPERTY(BlueprintReadOnly, Category = "LDW Base State")
		ULDWStateMachineComponent* OwnerStateMachineComp;

	UPROPERTY(BlueprintReadOnly, Category = "LDW Base State")
		TArray<FStateHistory> StateHistoryList;
	UPROPERTY(BlueprintReadOnly, Category = "LDW Base State")
		FDateTime EnteredStateTime;
	UPROPERTY(BlueprintReadOnly, Category = "LDW Base State")
		FDateTime ExitedStateTime;
	UPROPERTY(BlueprintReadOnly, Category = "LDW Base State")
		bool bIsInState;

public:
	ULDWBaseState();

	/** Getters, setters */
	UFUNCTION(BlueprintPure, Category = "LDW Base State")
		const FString& GetStateID() { return StateID; } 
	UFUNCTION(BlueprintPure, Category = "LDW Base State")
		const FGameplayTagContainer& GetStateTagContainer() { return StateTagContainer; }
	UFUNCTION(BlueprintPure, Category = "LDW Base State")
		bool GetIsInState() { return bIsInState; }

	/** Executes to initialize state */
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void InitState(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor);
	virtual void InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor);

	/** Executes when get in state */
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void EnterState();
	virtual void EnterState_Implementation();
	
	/** Executes every frame when in state */
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void TickState(float DeltaTime);
	virtual void TickState_Implementation(float DeltaTime);

	/** Executes when out state */
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void ExitState();
	virtual void ExitState_Implementation();

	/** Gets time in seconds from the last entered state to now. 0.0 if never entered state */
	UFUNCTION(BlueprintCallable, Category = "LDW Base State")
		float GetStateTime_LastEnteredToNow();
	
	/** Gets time in seconds from the last exited state to now. 0.0 if never entered state */
	UFUNCTION(BlueprintCallable, Category = "LDW Base State")
		float GetStateTime_LastExitedToNow();

	/** Gets the last duration of state. 0.0 if never entered or currently being in state */
	UFUNCTION(BlueprintCallable, Category = "LDW Base State")
		float GetStateTime_LastDuration();

	/** Handles action using ActionID string */
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void HandleActionString(const FString& ActionID);
	virtual void HandleActionString_Implementation(const FString& ActionID);
	
	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void SetLastExitedTimeToNow();
	virtual void SetLastExitedTimeToNow_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "LDW Base State")
		void SetLastEnteredTimeToNow();
	virtual void SetLastEnteredTimeToNow_Implementation();

protected:
	/** Changes to new state */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LDW Base State")
		void ChangeState(const FString& NewStateID);
	virtual void ChangeState_Implementation(const FString& NewStateID);
};
