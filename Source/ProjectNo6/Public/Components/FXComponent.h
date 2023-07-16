// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomData.h"
#include "Engine/EngineTypes.h"
#include "FXComponent.generated.h"

class UFXSystemComponent;
class UCurveFloat;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMovedAlongSpline, USplineComponent*, SplineToMoveAlongWith, const FVector&, Location, float, CurrentTime);

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "FX Component")
		FOnMovedAlongSpline OnStartedMovingAlongSplineDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "FX Component")
		FOnMovedAlongSpline OnEndedMovingAlongSplineDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "FX Component")
		FOnMovedAlongSpline OnUpdatedMovingAlongSplineDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "FX Component")
		UWidgetComponent* SparkleWidgetComp;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX Component")
		TArray<FCustomFX> CustomFXArray;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX Component")
		float SplineMovingInterval = 0.05f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX Component")
		TSoftObjectPtr<UCurveFloat> EaseInOutFloatCurve;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX Component")
		TArray<FSplineTrail> SplineTrails;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "FX Component")
		TSoftClassPtr<UUserWidget> SparkleWidget;

	FTimerHandle SplineMovingTimerHandle;
	bool bIsMovingAlongSpline;
	float SplineMovingTimeCounter;
	USplineComponent* CurrentUsedSplineComp;
	UFXSystemComponent* CurrentFXMovingAlongSpline;
	FVector LastFXLocation;

public:	
	UFXComponent();

	UFUNCTION(BlueprintCallable, Category = "FX Comp")
		bool GetIsMovingAlongSpline() { return bIsMovingAlongSpline; }

	/**
	 * Spawns a cascade system or niagara system from CustomFX.
	 * @param SpawnLocation - Location in world space or local space that will be used to spawn FX.
	 * @param SpawnRotation - Rotation in world space or local space that will be used to spawn FX.
	 * @param bInLocalSpace - Defines whether to spawn FX in world space or local space.
	 * @param PoolingMethod - Defines pooling method used to spawn FX. Automatically remaps to ENCPoolMethod (from EPSCPoolMethod) CustomFX has niagara FX type.
	 */
	UFUNCTION(BlueprintCallable, Category = "FX Component")
		UFXSystemComponent* SpawnFXAtLocation(const FString& FXId, const FVector& SpawnLocation, const FRotator& SpawnRotation, FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f), bool bInLocalSpace = true, bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None, bool AutoActivate = true);
	UFUNCTION(BlueprintCallable, Category = "FX Component")
		UFXSystemComponent* SpawnFXAttached(const FString& FXId, const FVector& SpawnLocation, const FRotator& SpawnRotation, FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f), bool bInLocalSpace = true, USceneComponent* ComponentToAttach = nullptr, FName SocketName = NAME_None, EAttachLocation::Type AttachLocationRule = EAttachLocation::KeepWorldPosition, bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None, bool AutoActivate = true);
	
	UFUNCTION(BlueprintCallable, Category = "FX Component")
		bool SpawnMovingFX(FString TrailId, FString FXId, float Duration = 0.5f);

protected:
	/** UActorComponent override functions */
	virtual void BeginPlay() override;

	/** Create SplineComponents to along with */
	void CreateSplineComponents();
	void CreateSparkleWidgetComp();
};
