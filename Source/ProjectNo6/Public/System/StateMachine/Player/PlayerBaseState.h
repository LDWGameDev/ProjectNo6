// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LDWBaseState.h"
#include "PlayerBaseState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateStartedCooldownSignature, UPlayerBaseState*, StateStartedCooldown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEnteredSignature, UPlayerBaseState*, EnteredState);

class UStatComponent;
class ACharacterPlayerBase;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNO6_API UPlayerBaseState : public ULDWBaseState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Base State")
		int32 PowerRequired = -1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Base State")
		bool bExitStartDelayRegeneratePower = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Base State")
		bool bEnterStopRegeneratePower = true;

	UPROPERTY(BlueprintAssignable, Category = "Player Base State")
		FOnStateStartedCooldownSignature OnStateStartedCooldownDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Player Base State")
		FOnStateEnteredSignature OnStateEnteredDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player Base State")
		ACharacterPlayerBase* OwnerCharacterPlayerBase;

	UPROPERTY(BlueprintReadWrite, Category = "Player Base State")
		bool bIsStateValid;

	UPROPERTY(BlueprintReadWrite, Category = "Player Base State")
		bool bCachedDashInput;
	UPROPERTY(BlueprintReadWrite, Category = "Player Base State")
		bool bCachedLAInput;
	UPROPERTY(BlueprintReadWrite, Category = "Player Base State")
		bool bCachedHAInput;

public:
	/** ULDWBaseState override functions */
	virtual void InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor) override;
	virtual void EnterState_Implementation() override;
	virtual void TickState_Implementation(float DeltaTime) override;
	virtual void ExitState_Implementation() override;
	virtual void HandleActionString_Implementation(const FString& ActionID) override;
	virtual void ChangeState_Implementation(const FString& NewStateID) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputActionTriggered_Movement(const FVector2D& InputValue);
	virtual void OnInputActionTriggered_Movement_Implementation(const FVector2D& InputValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputActionTriggered_Dash(bool InputValue);
	virtual void OnInputActionTriggered_Dash_Implementation(bool InputValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_LightAttack_Started();
	virtual void OnInputAction_LightAttack_Started_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_LightAttack_Triggered();
	virtual void OnInputAction_LightAttack_Triggered_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_LightAttack_Completed();
	virtual void OnInputAction_LightAttack_Completed_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_LightAttack_Canceled();
	virtual void OnInputAction_LightAttack_Canceled_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_HeavyAttack_Started();
	virtual void OnInputAction_HeavyAttack_Started_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_HeavyAttack_Triggered();
	virtual void OnInputAction_HeavyAttack_Triggered_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_HeavyAttack_Completed();
	virtual void OnInputAction_HeavyAttack_Completed_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Player Base State")
		void OnInputAction_HeavyAttack_Canceled();
	virtual void OnInputAction_HeavyAttack_Canceled_Implementation();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player Base State")
		virtual bool CheckPowerRequired();

	/** Decrease power stat value according to checked value. Make sure CheckPowerRequired() return true before executing this function */
	UFUNCTION(BlueprintCallable, Category = "Player Base State")
		void ApplyPowerRequired();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Base State")
		bool CheckValidState();
	virtual bool CheckValidState_Implementation();

protected:
	UFUNCTION(BlueprintCallable, Category = "Player Base State")
		void ClearCachedInputs();
};