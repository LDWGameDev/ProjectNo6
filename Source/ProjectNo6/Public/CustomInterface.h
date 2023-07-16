// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCustomInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTNO6_API ICustomInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/************************************************************************/
	/* Character's State Interface Functions                                                                    
	/************************************************************************/
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Interface : State")
	//	float GetCooldownTime();
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Interface : State")
	//	int32 GetPowerRequired();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void TriggerInteractionKey(APlayerController* PlayerControllerDoTrigger, const FKey& Key, bool& bResult);
};
