// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/StateMachine/Player/PlayerHumanBaseState.h"
#include "PlayerHumanLocomotionState.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UPlayerHumanLocomotionState : public UPlayerHumanBaseState
{
	GENERATED_BODY()

private:
	float cHoldTimeToLightAttackDash = 0.2f; 

public:
	UPlayerHumanLocomotionState();

	/** UPlayerHumanBaseState override functions */
	virtual void EnterState_Implementation() override;
	virtual void TickState_Implementation(float DeltaTime) override;
	virtual void ExitState_Implementation() override;

	virtual void OnInputActionTriggered_Movement_Implementation(const FVector2D& InputValue) override;
	virtual void OnInputActionTriggered_Dash_Implementation(bool ShouldDash) override;

	virtual void OnInputAction_LightAttack_Started_Implementation() override;
	virtual void OnInputAction_LightAttack_Canceled_Implementation() override;
	virtual void OnInputAction_LightAttack_Completed_Implementation() override;

	virtual void OnInputAction_HeavyAttack_Started_Implementation() override;
};
