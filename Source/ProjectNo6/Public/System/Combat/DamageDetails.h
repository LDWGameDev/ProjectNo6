// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageDetails.generated.h"

class UCombatMove;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNO6_API UDamageDetails : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Damage Details")
		AActor* InstigatorActor;
	UPROPERTY(BlueprintReadWrite, Category = "Damage Details")
		AActor* TargetActor;
	UPROPERTY(BlueprintReadWrite, Category = "Damage Details")
		UCombatMove* InstigatorCombatMove;
	UPROPERTY(BlueprintReadWrite, Category = "Damage Details")
		int32 Damage;

public:
	UDamageDetails();

	UFUNCTION(BlueprintPure)
		bool IsDamageDetailsValid() {return (InstigatorActor && TargetActor && InstigatorCombatMove);}
};
