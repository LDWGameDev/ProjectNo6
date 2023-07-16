// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UCustomFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Calculates and returns the array of points that are arranged using sunflower seed method
	 * @param BoundarySmooth		Smooth value of the boundary. 0 gives the typical sunflower seed arrangement but jagged boundary. 2 is kind of the max value of this, gives a smoother boundary.
	 *								Value range from 0 to 2.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom Function Library")
		static void SunFlowerSeedArrangement(TArray<FVector2D>& OutSeedLocationArray, int32 SeedNum, float BoundarySmooth = 0.0f);

	/** Returns angle between 2 vector. Value range from -180 to 180 */
	UFUNCTION(BlueprintCallable, Category = "Custom Function Library")
		static float AngleBetween2Vector(const FVector& Start, const FVector& End);

	/**
	 * Custom spawn sound at specified location using function of game instance
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom Function Library", meta = (WorldContext = WorldContextObject))
		static void CustomSpawnSoundAtLocation(UObject* WorldContextObject, FString SoundId, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Custom Function Library")
		static UCustomGameUserSettings* GetCustomGameUserSettings();

	UFUNCTION(BlueprintPure, Category = "Custom Fucntion Library")
		static void GenerateRandomUniqueIndexesArray(int32 NumberOfIndexes, int32 MaxElementValue, TArray<int32>& OutIndexesArray);

	UFUNCTION(BlueprintPure, Category = "Custom Function Library")
		static void GetDurationInSecondsBetween2DateTime(float& OutDurationInSeconds, const FDateTime& StartDate, const FDateTime& EndDate);

	UFUNCTION(BlueprintPure, Category = "Custom Function Library")
		static void GetDayOfWeek(const FDateTime& DateToGet, FString& OutDayOfWeek);
};
