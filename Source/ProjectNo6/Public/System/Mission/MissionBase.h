// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MissionBase.generated.h"

class UGameInstance;
class USoundBase;
class USoundConcurrency;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionFinishedSignature, UMissionBase*, CompletedMission);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTNO6_API UMissionBase : public UObject
{
	GENERATED_BODY()

public:
	FOnMissionFinishedSignature OnMissionFinishedDelegateS;

public:
	UMissionBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnStarted();

protected:
	UFUNCTION(BlueprintPure)
		UGameInstance* CustomGetGameInstance();
	UFUNCTION(BlueprintCallable)
		void CustomPlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, USoundConcurrency* ConcurrencySettings = nullptr, const AActor* OwningActor = nullptr, bool bIsUISound = true);
	UFUNCTION(BlueprintCallable)
		APlayerController* CustomGetPlayerController();
};
