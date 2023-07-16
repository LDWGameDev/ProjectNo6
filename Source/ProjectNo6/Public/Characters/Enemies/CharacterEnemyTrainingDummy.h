// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/CharacterEnemyBase.h"
#include "CharacterEnemyTrainingDummy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNO6_API ACharacterEnemyTrainingDummy : public ACharacterEnemyBase
{
	GENERATED_BODY()
	
public:
	ACharacterEnemyTrainingDummy();

	// ACharacterEnemyBase override functions
	virtual void HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails) override;
};
