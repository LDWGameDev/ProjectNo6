// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovingFXHandler.generated.h"

class UFXSystemComponent;
class USplineComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UMovingFXHandler : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		UFXSystemComponent* MovingFXSystem;
	UPROPERTY()
		USplineComponent* TrajectorySpline;
	UPROPERTY()
		UCurveFloat* FloatCurve;
	UPROPERTY()
		AActor* OwnerActor;
	FTimerHandle HandleMovingFX;
	FVector LastFXLocation;
	float SplineTimeCounter;
	float Interval;

public:
	void StartMovingWithSpline(AActor* Owner, UFXSystemComponent* FXToMove, USplineComponent* Trajectory, UCurveFloat* FloatCurveVariation, float Duration = 0.3f, float TimerInterval = 0.05f);

private:
	UFUNCTION()
		void UpdateMovingWithSpline();
	void EndMovingWithSpline();
};
