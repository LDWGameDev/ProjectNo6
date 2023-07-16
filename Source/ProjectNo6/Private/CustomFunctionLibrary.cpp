// Copy Right LDW. All Rights Reserved.


#include "CustomFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstanceCustom.h"
#include "System/Settings/CustomGameUserSettings.h"

void UCustomFunctionLibrary::SunFlowerSeedArrangement(TArray<FVector2D>& OutSeedLocationArray, int32 SeedNum, float BoundarySmooth /*= 0.0f*/)
{
	if (SeedNum < 0) return;
	
	// Golden ratio
	float Phi = (1 + UKismetMathLibrary::Sqrt(5.0f)) * 0.5f;
	// Value used to calculate theta for each points
	float AngleStride = 2 * PI / FMath::Pow(Phi, 2.0f);
	int32 BoundaryPointsNum = FMath::RoundToInt32(BoundarySmooth * UKismetMathLibrary::Sqrt(SeedNum));

	for (int i = 1; i < SeedNum + 1; i++)
	{
		float Radius = 1.0f;
		if (i <= SeedNum - BoundaryPointsNum)
		{
			Radius = UKismetMathLibrary::Sqrt(i - 0.5f) / UKismetMathLibrary::Sqrt(SeedNum - (BoundaryPointsNum + 1) * 0.5f);
		}
		float Theta = i * AngleStride;
		FVector2D NewPoint = FVector2D((Radius * UKismetMathLibrary::Cos(Theta)), Radius * UKismetMathLibrary::Sin(Theta));
		OutSeedLocationArray.Add(NewPoint);
	}
}

float UCustomFunctionLibrary::AngleBetween2Vector(const FVector& Start, const FVector& End)
{
	FVector StartNormarlized = Start;
	StartNormarlized.Normalize();
	FVector EndNormalized = End;
	EndNormalized.Normalize();

	if (UKismetMathLibrary::Vector_IsNearlyZero(StartNormarlized, 0.001f) || UKismetMathLibrary::Vector_IsNearlyZero(EndNormalized, 0.001f)) return 0.0f;
	float AngleResult = FMath::RadiansToDegrees(acosf(FVector::DotProduct(StartNormarlized, EndNormalized)));
	AngleResult = FMath::Clamp(AngleResult, 0.0f, 180.0f);
	return ((FVector::CrossProduct(StartNormarlized, EndNormalized).Z > 0) ? AngleResult : -AngleResult);
}

void UCustomFunctionLibrary::CustomSpawnSoundAtLocation(UObject* WorldContextObject, FString SoundId, FVector Location)
{
	if (!WorldContextObject) return;
	if (UGameInstanceCustom* CustomGameInstance = Cast<UGameInstanceCustom>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		CustomGameInstance->CustomSpawnSoundAtLocation(SoundId, Location);
	}
}

UCustomGameUserSettings* UCustomFunctionLibrary::GetCustomGameUserSettings()
{
	return UCustomGameUserSettings::Get();
}

void UCustomFunctionLibrary::GenerateRandomUniqueIndexesArray(int32 NumberOfIndexes, int32 MaxElementValue, TArray<int32>& OutIndexesArray)
{
	OutIndexesArray.Empty();
	if (MaxElementValue >= NumberOfIndexes - 1)
	{
		TArray<int32> AllIndexesArray;
		for (int32 i = 0; i <= MaxElementValue; i++)
		{
			AllIndexesArray.Add(i);
		}
		
		for (int32 i = 0; i < NumberOfIndexes; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, AllIndexesArray.Num() - 1);
			OutIndexesArray.Add(AllIndexesArray[RandomIndex]);
			AllIndexesArray.RemoveAt(RandomIndex, 1, true);
		}
	}
}

void UCustomFunctionLibrary::GetDurationInSecondsBetween2DateTime(float& OutDurationInSeconds, const FDateTime& StartDate, const FDateTime& EndDate)
{
	FTimespan Duration = EndDate - StartDate;
	OutDurationInSeconds = Duration.GetSeconds();
}

void UCustomFunctionLibrary::GetDayOfWeek(const FDateTime& DateToGet, FString& OutDayOfWeek)
{
	EDayOfWeek DayOfWeek = DateToGet.GetDayOfWeek();
	switch (DayOfWeek)
	{
		case EDayOfWeek::Monday: 
		{
			OutDayOfWeek = TEXT("Mon");
			break;
		}
		case EDayOfWeek::Tuesday:
		{
			OutDayOfWeek = TEXT("Tue");
			break;
		}
		case EDayOfWeek::Wednesday:
		{
			OutDayOfWeek = TEXT("Wed");
			break;
		}
		case EDayOfWeek::Thursday:
		{
			OutDayOfWeek = TEXT("Thur");
			break;
		}
		case EDayOfWeek::Friday:
		{
			OutDayOfWeek = TEXT("Fri");
			break;
		}
		case EDayOfWeek::Saturday:
		{
			OutDayOfWeek = TEXT("Sat");
			break;
		}
		case EDayOfWeek::Sunday:
		{
			OutDayOfWeek = TEXT("Sun");
			break;
		}
	}
}
