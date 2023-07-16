// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/AnimInstancePlayerBase.h"
#include "AnimInstancePlayerHuman.generated.h"

class ACharacterPlayerHuman;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UAnimInstancePlayerHuman : public UAnimInstancePlayerBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Human")
		ACharacterPlayerHuman* OwnerPlayerHuman;

public:
	/** UAnimInstancePlayerBase override functions */
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
