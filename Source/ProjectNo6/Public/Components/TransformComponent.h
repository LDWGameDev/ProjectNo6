// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "CustomData.h"
#include "TransformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UTransformComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Alpha float curve array. Adds 1 linear alpha float curve to create timelines */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform Component")
		TArray<FAlphaCurve> AlphaCurveArray;

private:
	/** Location duration control relating variables */
	UPROPERTY()
		FTimeline LocationTimeline;
	FVector SaveLocation;
	FVector SaveNewLocation;
	bool bSaveSweepLocation;
	ETeleportType SaveTeleportTypeLocation;
	FHitResult SweepHitResultLocation;
	UPROPERTY()
		UCurveFloat* SaveAlphaCurveFloatLocation;

	/** Rotation duration control relating variables */
	UPROPERTY()
		FTimeline RotationTimeline;
	FRotator SaveRotation;
	FRotator SaveNewRotation;
	ETeleportType SaveTeleportTypeRotation;
	UPROPERTY()
		UCurveFloat* SaveAlphaCurveFloatRotation;

	/** Save FHitResult when executes custom set owner actor location function */
	FHitResult SaveSetOwnerLocationHitResult;

	/** Boolean value indicates if can ever tick timelines. Default value is true, if create timelines fail, set it to false */
	bool bCanEverTickTimelines;

protected:
	/** AActorComponent override functions */
	virtual void BeginPlay() override;

public:	
	UTransformComponent();

	/** AActorComponent override functions */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Moves owner actor to a specified location in a duration
	 * @param	AlphaCurveType determines which alpha curve type will be used to lerp location. Default and if cannot find other type will be linear 
	 */
	UFUNCTION(BlueprintCallable, Category = "Transform Component")
		void MoveToLocation(const FVector& NewLocation, float Duration, EAlphaCurveType AlphaCurveType = EAlphaCurveType::Linear, bool DoSweepLocation = false, ETeleportType TeleportType = ETeleportType::None);
	
	/**
	 * Rotates owner actor to a specified rotation in a duration
	 * @param	AlphaCurveType determines which alpha curve type will be used to lerp rotation. Default and if cannot find other type will be linear
	 */
	UFUNCTION(BlueprintCallable, Category = "Transform Component")
		void RotateToRotation(const FRotator& NewRotation, float Duration, EAlphaCurveType AlphaCurveType = EAlphaCurveType::Linear, ETeleportType TeleportType = ETeleportType::None);

	/** Custom function to set owner actor location */
	UFUNCTION(BlueprintCallable, Category = "Transform Component")
		void SetOwnerActorLocation(const FVector& NewLocation, bool ShouldBreakDurationControl, bool DoSweepLocation = false, ETeleportType TeleportType = ETeleportType::None);

	/** Custom function to set owner actor rotation */
	UFUNCTION(BlueprintCallable, Category = "Transform Component")
		void SetOwnerActorRotation(const FRotator& NewRotation, bool ShouldBreakDurationControl, ETeleportType TeleportType = ETeleportType::None);
	
	/** Stop controlling location and rotation */
	UFUNCTION(BlueprintCallable, Category = "Transform Component")
		void StopControllingTransform(bool bStopLocation, bool bStopRotation);

	/** Returns a boolean indicates whether is controlling location */
	UFUNCTION(BlueprintPure, Category = "Transform Component")
		bool IsControllingLocation() { return LocationTimeline.IsPlaying(); }
	/** Returns a boolean indicates whether is controlling rotation */
	UFUNCTION(BlueprintPure, Category = "Transform Component")
		bool IsControllingRotation() { return RotationTimeline.IsPlaying(); }

private:
	/** Creates timelines handling transform */
	void CreateTimelines();
	/** Ticks timelines handling transform */
	void TickTimelines(float DeltaTime);

	/** Timeline handling functions */
	UFUNCTION()
		void HandleTimeline_Location(float TimeValue);
	UFUNCTION()
		void HandleTimeline_Rotation(float TimeValue);

	/** Return dereferenced soft pointer of a float curve if it is valid from AlphaCurveArray */
	UCurveFloat* GetAlphaCurveFloat(EAlphaCurveType AlphaCurveType);
};
