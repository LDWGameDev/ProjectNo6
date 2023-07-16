// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/StateMachine/Player/PlayerBaseState.h"
#include "PlayerHumanBaseState.generated.h"

class ACharacterPlayerHuman;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UPlayerHumanBaseState : public UPlayerBaseState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Human Base State")
		bool bAutoHandleDashState = true;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Player Human Base State")
		bool bCanDash = false;
	UPROPERTY(BlueprintReadOnly)
		ACharacterPlayerHuman* OwnerCharacterPlayerHuman;

public:
	UPlayerHumanBaseState();
	
	/** UPlayerBaseState override functions */
	virtual void InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor) override;
	virtual void HandleActionString_Implementation(const FString& ActionID) override;
	virtual bool CheckValidState_Implementation() override;

public:
	virtual void OnInputActionTriggered_Dash_Implementation(bool InputValue) override;
};
