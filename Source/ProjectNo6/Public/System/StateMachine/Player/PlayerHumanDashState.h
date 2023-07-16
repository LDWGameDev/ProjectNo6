// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/StateMachine/Player/PlayerHumanBaseState.h"
#include "PlayerHumanDashState.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UPlayerHumanDashState : public UPlayerHumanBaseState
{
	GENERATED_BODY()

public:
	UPlayerHumanDashState();

	/** UPlayerHumanBaseState override functions */
	virtual void EnterState_Implementation() override;
	virtual void TickState_Implementation(float DeltaTime) override;
	virtual void ExitState_Implementation() override;
};
