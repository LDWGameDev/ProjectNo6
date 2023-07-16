// Copy Right LDW. All Rights Reserved.


#include "Components/MovingFXHandler.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"

void UMovingFXHandler::StartMovingWithSpline(AActor* Owner, UFXSystemComponent* FXToMove, USplineComponent* Trajectory, UCurveFloat* FloatCurveVariation, float Duration /*= 0.3f*/, float TimerInterval /*= 0.05f*/)
{
	if (!FXToMove || !Trajectory || Duration <= 0.0f || TimerInterval <= 0.0f || !Owner || !FloatCurveVariation)
	{
		EndMovingWithSpline();
		return;
	}
	MovingFXSystem = FXToMove;
	TrajectorySpline = Trajectory;
	Interval = TimerInterval;
	OwnerActor = Owner;
	FloatCurve = FloatCurveVariation;

	SplineTimeCounter = 0.0f;
	FVector StartLocation = TrajectorySpline->GetLocationAtTime(SplineTimeCounter, ESplineCoordinateSpace::World);
	FXToMove->SetWorldLocation(StartLocation);
	LastFXLocation = StartLocation;
	TrajectorySpline->Duration = Duration;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HandleMovingFX, this, &UMovingFXHandler::UpdateMovingWithSpline, TimerInterval, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MovingFXHandler GetWorld() NOT valid"));
	}
}	

void UMovingFXHandler::UpdateMovingWithSpline()
{
	if (!MovingFXSystem || !TrajectorySpline || !FloatCurve || !OwnerActor)
	{
		EndMovingWithSpline();
		return;
	}
	SplineTimeCounter += Interval;

	if (SplineTimeCounter < TrajectorySpline->Duration)
	{
		// Set FX location
		float SplineTime = SplineTimeCounter;
		float AlphaMapped = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, TrajectorySpline->Duration), FVector2D(0.0f, 1.0f), SplineTime);
		float CurveAlpha = FloatCurve->GetFloatValue(AlphaMapped);
		SplineTime = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(0.0f, TrajectorySpline->Duration), CurveAlpha);
		FVector NewLocation = TrajectorySpline->GetLocationAtTime(SplineTime, ESplineCoordinateSpace::World);
		MovingFXSystem->SetWorldLocation(NewLocation);

		// Set FX rotation
		FVector FXUpVector = NewLocation - OwnerActor->GetActorLocation();
		FXUpVector.Normalize();
		FVector FXForwardVector = NewLocation - LastFXLocation;
		FXForwardVector.Normalize();
		MovingFXSystem->SetWorldRotation(FRotationMatrix::MakeFromXZ(FXForwardVector, FXUpVector).Rotator());
		LastFXLocation = NewLocation;
	}
	else
	{
		EndMovingWithSpline();
	}
}

void UMovingFXHandler::EndMovingWithSpline()
{
	if (TrajectorySpline)
	{
		FVector EndLocation = TrajectorySpline->GetLocationAtTime(TrajectorySpline->Duration, ESplineCoordinateSpace::World);
		MovingFXSystem->SetWorldLocation(EndLocation);
	}

	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(HandleMovingFX))
		{
			GetWorld()->GetTimerManager().ClearTimer(HandleMovingFX);
		}

		TWeakObjectPtr<UFXSystemComponent> FXToDestroy = MovingFXSystem;
		TWeakObjectPtr<UObject> SelfWeak = this;
		FTimerHandle DelayDestroyFXTimerHandle;
		FTimerDelegate DelayDestroyFXTimerDelegate;
		DelayDestroyFXTimerDelegate.BindLambda([FXToDestroy, SelfWeak]() {
			if (FXToDestroy.IsValid())
			{
				FXToDestroy.Get()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				FXToDestroy.Get()->DestroyComponent();
			}
			if (SelfWeak.IsValid())
			{
				SelfWeak->MarkAsGarbage();
			}
			});
		GetWorld()->GetTimerManager().SetTimer(DelayDestroyFXTimerHandle, DelayDestroyFXTimerDelegate, 0.2f, false);
	}
}
