// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/Combat/CombatMove.h"
#include "CombatMoveSphereOverlap.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class PROJECTNO6_API UCombatMoveSphereOverlap : public UCombatMove
{
	GENERATED_BODY()
	
public:
	/** Sphere overlap variables */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap")
		FVector OverlapLocation;
	/** Determines whether to check in local coordinate. If is in local, transform OverlapLocation to desired world location before trigger this move */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap")
		bool bInLocalCoordinate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move Sphere Overlap")
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move Sphere Overlap")
		float SphereRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move Sphere Overlap")
		TSoftClassPtr<AActor> ActorClassFilterSoftClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap : Debug")
		bool bDebugSphereOverlap = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap : Debug", meta = (EditConditionHides, EditCondition = "bDebugSphereOverlap"))
		float SphereOverlapDebugTime = 5.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap : Debug", meta = (EditConditionHides, EditCondition = "bDebugSphereOverlap"))
		FLinearColor HitColor = FLinearColor::Green;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat Move Sphere Overlap : Debug", meta = (EditConditionHides, EditCondition = "bDebugSphereOverlap"))
		FLinearColor NotHitColor = FLinearColor::Red;

public:
	UCombatMoveSphereOverlap();

	/** UCombatMove override functions */
	virtual void OnMoveTriggered_Implementation() override;
	virtual void DealDamageToTargets_Implementation(const TArray<AActor*>& TargetActorArray) override;

	UFUNCTION(BlueprintCallable, Category = "Combat Move Sphere Overlap")
		void SphereOverlapSetWorldLocation(const FVector& WorldLocation);
};
