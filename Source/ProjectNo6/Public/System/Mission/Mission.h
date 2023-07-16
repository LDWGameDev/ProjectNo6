// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Mission.generated.h"

class UGameInstance;
class APlayerController;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTNO6_API UMission : public UObject
{
	GENERATED_BODY()

public:
	UMission();
	
protected:
	UFUNCTION(BlueprintPure)
		UGameInstance* CustomGetGameInstance();
	UFUNCTION(BlueprintCallable)
		void CustomPlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, USoundConcurrency* ConcurrencySettings = nullptr, const AActor* OwningActor = nullptr, bool bIsUISound = true);
	UFUNCTION(BlueprintCallable)
		APlayerController* CustomGetPlayerController();
};
