// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstancePlayerBase.generated.h"

class ACharacterPlayerBase;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UAnimInstancePlayerBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		ACharacterPlayerBase* OwnerCharacterPlayerBase;
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		float MovementSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		int32 AnimationIndex = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		float MovingSpeedAlpha;
	UPROPERTY(BlueprintReadOnly, Category = "Anim Instance Player Base")
		float MovingDirectionAngle;

public:
	/** UAnimInstance override */
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
}; 
