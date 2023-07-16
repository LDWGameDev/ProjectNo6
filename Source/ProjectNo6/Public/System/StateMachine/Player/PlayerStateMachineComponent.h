// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LDWStateMachineComponent.h"
#include "PlayerStateMachineComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UPlayerStateMachineComponent : public ULDWStateMachineComponent
{
	GENERATED_BODY()
	
public:
	UPlayerStateMachineComponent();
	
	UFUNCTION(BlueprintPure, Category = "Player State Machine Comp")
		UPlayerBaseState* GetState(FString StateID);
};
