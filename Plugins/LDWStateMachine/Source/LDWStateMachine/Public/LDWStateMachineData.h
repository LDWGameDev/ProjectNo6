// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include "LDWStateMachineData.generated.h"


/**
 * Struct contains information about state history
 */
USTRUCT(BlueprintType)
struct FStateHistory
{
	GENERATED_BODY()

public:
	FDateTime EnteredTime;	
	FDateTime ExitedTime;

	FStateHistory() : EnteredTime(FDateTime::UtcNow()), ExitedTime(FDateTime::UtcNow()) {}
	FStateHistory(const FDateTime& Entered, const FDateTime& Exited) : EnteredTime(Entered), ExitedTime(Exited) { }

	/** Duration of state in seconds */
	float GetDuration() { return (ExitedTime - EnteredTime).GetTotalSeconds(); }
};


/**
 * Class contains state machine plugin data like structs and so on
 */
UCLASS()
class LDWSTATEMACHINE_API ULDWStateMachineData : public UObject
{
	GENERATED_BODY()

};
