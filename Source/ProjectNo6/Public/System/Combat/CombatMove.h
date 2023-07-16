// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CustomData.h"
#include "CombatMove.generated.h"

class UCombatComponent;
class UStatComponent;

/**
 * CombatMove can only regconize an actor that has UGameplayTagComponent with OwnerTagContainer HasTag() TargetTagContainer returns true in order to take effects.
 */
UCLASS(Blueprintable, NotBlueprintType, DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UCombatMove : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move")
		FString MoveID;
	// Determines which tags an actor can have to be able to be recognize by this move
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move")
		FGameplayTagContainer TargetTagContainer;
	// Array of FXs that will be spawned right after combat move hits target
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move")
		TArray<FHitTargetFX> HitTargetFXArray;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move")
		FString HitTargetSF;

	// Hit target type
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit")
		EHitType HitType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move : Hit")
		TArray<FMoveStat> MoveStatArray;
	UPROPERTY(BlueprintReadWrite, Category = "Combat Move : Hit")
		UStatComponent* StatCompToGetFrom;

	//// Types of hit target location control
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control")
	//	ETargetLocationControlType TargetLocationControlType;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	bool bCircleDistributionInLocal = true;
	//// Center location to calculate circle distribution. This location is in local coordinate according to the owner's transform.
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	FVector CircleDistributionCenterLocation;
	//// Determines the max distance that target can be moved toward distributed location.
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	float CircleDistributionControlMaxDistance = -1.0f;
	//// Radius of circle distribution
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	float CircleDistributionRadius = 100.0f;
	//// Smoothness of the boundary of the circle. Value range should be from 0 to 2.
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	float CircleDistributionBoundarySmooth = 0.0f;

	//// Whether to use force direction in local space, based on owner transform or world space
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution"))
	//	bool bDFADInLocal = true;
	//// Direction of force vector
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution"))
	//	FVector DFADDirection;
	//// Magnitude of force vector. Random between X and Y
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution"))
	//	FVector2D DFADStrength;
	//// Maximum angle to distribute targets
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution"))
	//	float DFADMaxAngle;
	//// Maximum angle between targets
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution"))
	//	float DFADMaxAngleBetweenTargets;
		
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialDistribution"))
	//	bool bRadialDistributionInLocal = true;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialDistribution"))
	//	FVector RadialDistributionLocation;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialDistribution"))
	//	float RadialDistributionRadius;
	//// Variation applied to distributed location
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialDistribution"))
	//	FVector RadialDistributionVariation;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialForce"))
	//	bool bRadialForceInLocal = true;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialForce"))
	//	FVector RadialForceLocation;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialForce"))
	//	float RadialForceAngle;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Location Control", meta = (EditConditionHides, EditCondition = "TargetLocationControlType == ETargetLocationControlType::RadialForce"))
	//	FVector2D RadialForceStrength;

	//// Location control type
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Hit Target Rotation Control")
	//	ETargetRotationControlType TargetRotationControlType;

	//// Debug variables
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Debug")
	//	bool bDebugCombatMove;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Debug", meta = (EditConditionHides, EditCondition = "bDebugCombatMove"))
	//	float CombatMoveDebugTime = 5.0f;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Debug", meta = (EditConditionHides, EditCondition = "bDebugCombatMove"))
	//	FLinearColor CombatMoveDebugColor = FLinearColor::Blue;
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move : Debug", meta = (EditConditionHides, EditCondition = "bDebugCombatMove && TargetLocationControlType == ETargetLocationControlType::CircleDistribution"))
	//	float CircleDistributionDebugRadius = 5.0f;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Combat Move")
		UCombatComponent* OwnerCombatComp;

public:
	UCombatMove();

	/** Calls to initialize this move */
	UFUNCTION(BlueprintNativeEvent, Category = "Combat Move")
		void Initialize(UCombatComponent* OwnerCombatComponent);
	virtual void Initialize_Implementation(UCombatComponent* OwnerCombatComponent);

	/** Triggers this move, checks for targets in derived classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Combat Move")
		void OnMoveTriggered();
	virtual void OnMoveTriggered_Implementation();

	/**
	 * Deals damage to target actors. Overrides this function to apply debuffs or negative effects to targets
	 * Should have parameter like this TArray<AActor*>& TargetActorArray but when expose this function to blueprint, it doesn't work. Then remove the & and turn into something like this TArray<AActor*> TargetActorArray
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat Move")
		void DealDamageToTargets(const TArray<AActor*>& TargetActorArray);
	virtual void DealDamageToTargets_Implementation(const TArray<AActor*>& TargetActorArray);

//	/** Set in local and world location for circle distribution location control type */
//	UFUNCTION(BlueprintCallable, Category = "Combat Move")
//		void CircleDistributionSetWorldLocation(const FVector& WorldLocation);
//
//	/** Set in local and world direction for add force with direction location control type */
//	UFUNCTION(BlueprintCallable, Category = "Combat Move")
//		void AddForceWithDirectionSetWorldDirection(const FVector& WorldDirection);
//
//private:
//	/**
//	 * Find the closet location to target in 2 array of locations and target actors
//	 * 
//	 * @param TargetArray - Array of target actors
//	 * @param LocationArray - Array of distributed location
//	 * @param OutLocationArray - Array of location that is sorted in the most appropriate order for the targets. Target and appropriate location have the same index
//	 */
//	// void FindAppropriateLocationForTarget(const TArray<AActor*>& TargetArray, const TArray<FVector>& LocationArray, TArray<FVector>& OutLocationArray);
//
//	void ControlLocation(TArray<AActor*>& ActorArray);
//	void ControlRotation(TArray<AActor*>& ActorArray);
//
//	void ControlLocationCircleDistribution(TArray<AActor*>& ActorArray);
//	void ControlLocationDirectionalForceAngleDistribution(TArray<AActor*>& ActorArray);
//	void ControlLocationRadialDistribution(TArray<AActor*>& ActorArray);
//	void ControlLocationRadialForce(TArray<AActor*> ActorArray);

};
