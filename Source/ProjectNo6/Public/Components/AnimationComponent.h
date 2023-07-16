// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomData.h"
#include "AnimationComponent.generated.h"

/**
 * Should be added to ACharacter
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Component")
		TArray<FCustomMontage> CustomMontageList;
	UPROPERTY()
		UAnimInstance* OwnerAnimInstance;
	
	/** Holds last played custom montage that is playing right now or had been stopped */
	UPROPERTY()
		FCustomMontage LastPlayedCustomMontage;

public:	
	UAnimationComponent();

	/** Plays custom montage and returns the time it takes. Returns 0.0 if fail */
	UFUNCTION(BlueprintCallable, Category = "Animation Component")
		float PlayCustomMontage(const FString& MontageID, const FName& MontageSection, float PlayRate = 1.0f);

protected:
	/** UActorComponent override functions */
	virtual void BeginPlay() override;

	/** Returns a boolean value indicates whether can play a custom montage base on it priority to the current playing. Returns true if no montage is playing */
	bool CheckCanPlayCustomMontage(const FCustomMontage& CustomMontageToPlay);
};
