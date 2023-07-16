// Copy Right LDW. All Rights Reserved.


#include "Components/TransformComponent.h"
#include "System/AssetManager/CustomAssetManager.h"

UTransformComponent::UTransformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bCanEverTickTimelines = true;
}

void UTransformComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateTimelines();
	if (GetOwner())
	{
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector(1.0f, 1.0f, 1.0f));
	}
}

void UTransformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickTimelines(DeltaTime);
}

void UTransformComponent::MoveToLocation(const FVector& NewLocation, float Duration, EAlphaCurveType AlphaCurveType /*= EAlphaCurveType::Linear*/, bool DoSweepLocation /*= false*/, ETeleportType TeleportType /*= ETeleportType::None*/)
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (Duration <= 0.0f)
		{
			if (LocationTimeline.IsPlaying()) LocationTimeline.Stop();
			OwnerActor->SetActorLocation(NewLocation, DoSweepLocation, &SweepHitResultLocation, TeleportType);
		}
		else
		{
			// Return if cannot tick timeline
			if (!bCanEverTickTimelines) return;

			// Enable component tick if not already enabled
			if (!IsComponentTickEnabled()) SetComponentTickEnabled(true);

			// Reset then check for alpha curve type
			SaveAlphaCurveFloatLocation = GetAlphaCurveFloat(EAlphaCurveType::Linear);
			if (AlphaCurveType != EAlphaCurveType::Custom && AlphaCurveType != EAlphaCurveType::Linear)
			{
				if (UCurveFloat* AlphaCurveFloat = GetAlphaCurveFloat(AlphaCurveType))
				{
					SaveAlphaCurveFloatLocation = AlphaCurveFloat;
				}
			}

			SaveLocation = OwnerActor->GetActorLocation();
			SaveNewLocation = NewLocation;
			bSaveSweepLocation = DoSweepLocation;
			SaveTeleportTypeLocation = TeleportType;

			LocationTimeline.SetPlayRate(1.0f / Duration);
			LocationTimeline.PlayFromStart();
		}
	}
}

void UTransformComponent::RotateToRotation(const FRotator& NewRotation, float Duration, EAlphaCurveType AlphaCurveType /*= EAlphaCurveType::Linear*/, ETeleportType TeleportType /*= ETeleportType::None*/)
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (Duration <= 0.0f)
		{
			if (RotationTimeline.IsPlaying()) RotationTimeline.Stop();
			OwnerActor->SetActorRotation(NewRotation, TeleportType);
		}
		else
		{
			// Return if cannot tick timeline
			if (!bCanEverTickTimelines) return;

			// Enable component tick if not already enabled
			if (!IsComponentTickEnabled()) SetComponentTickEnabled(true);

			// Reset then check for alpha curve type
			SaveAlphaCurveFloatRotation = GetAlphaCurveFloat(EAlphaCurveType::Linear);
			if (AlphaCurveType != EAlphaCurveType::Custom && AlphaCurveType != EAlphaCurveType::Linear)
			{
				if (UCurveFloat* AlphaCurveFloat = GetAlphaCurveFloat(AlphaCurveType))
				{
					SaveAlphaCurveFloatRotation = AlphaCurveFloat;
				}
			}

			SaveRotation = OwnerActor->GetActorRotation();
			SaveNewRotation = NewRotation;
			SaveTeleportTypeRotation = TeleportType;

			RotationTimeline.SetPlayRate(1.0f / Duration);
			RotationTimeline.PlayFromStart();
		}
	}
}

void UTransformComponent::SetOwnerActorLocation(const FVector& NewLocation, bool ShouldBreakDurationControl, bool DoSweepLocation /*= false*/, ETeleportType TeleportType /*= ETeleportType::None*/)
{
	if (AActor* OwnerActor = GetOwner())
	{	
		if (LocationTimeline.IsPlaying())
		{
			if (ShouldBreakDurationControl)
			{
				LocationTimeline.Stop();
			}
			else
			{
				return;
			}
		}
		OwnerActor->SetActorLocation(NewLocation, DoSweepLocation, &SaveSetOwnerLocationHitResult, TeleportType);
	}
}

void UTransformComponent::SetOwnerActorRotation(const FRotator& NewRotation, bool ShouldBreakDurationControl, ETeleportType TeleportType /*= ETeleportType::None*/)
{	
	if (AActor* OwnerActor = GetOwner())
	{
		if (RotationTimeline.IsPlaying())
		{
			if (ShouldBreakDurationControl)
			{
				RotationTimeline.Stop();
			}
			else
			{
				return;
			}
		}
		OwnerActor->SetActorRotation(NewRotation, TeleportType);
	}
}

void UTransformComponent::StopControllingTransform(bool bStopLocation, bool bStopRotation)
{
	if (bStopLocation && LocationTimeline.IsPlaying())
	{
		LocationTimeline.Stop();
	}
	if (bStopRotation && RotationTimeline.IsPlaying())
	{
		RotationTimeline.Stop();
	}
}

void UTransformComponent::CreateTimelines()
{
	if (UCurveFloat* LinearAlphaCurve = GetAlphaCurveFloat(EAlphaCurveType::Linear))
	{
		// Create timeline handling location
		FOnTimelineFloatStatic LocationTimelineFloatStatic;
		LocationTimelineFloatStatic.BindUObject(this, &UTransformComponent::HandleTimeline_Location);
		LocationTimeline.AddInterpFloat(LinearAlphaCurve, LocationTimelineFloatStatic);
		LocationTimeline.SetLooping(false);
		LocationTimeline.SetTimelineLength(1.0f);

		// Create timeline handling rotation
		FOnTimelineFloatStatic RotationTimelineFloatStatic;
		RotationTimelineFloatStatic.BindUObject(this, &UTransformComponent::HandleTimeline_Rotation);
		RotationTimeline.AddInterpFloat(LinearAlphaCurve, RotationTimelineFloatStatic);
		RotationTimeline.SetLooping(false);
		RotationTimeline.SetTimelineLength(1.0f);
	}
}

void UTransformComponent::TickTimelines(float DeltaTime)
{
	if (!bCanEverTickTimelines) return;
	LocationTimeline.TickTimeline(DeltaTime);
	RotationTimeline.TickTimeline(DeltaTime);
	
	// Disable component tick if no timelines is playing
	if (IsComponentTickEnabled() && !LocationTimeline.IsPlaying() && !RotationTimeline.IsPlaying()) SetComponentTickEnabled(false);
}

void UTransformComponent::HandleTimeline_Location(float TimeValue)
{
	if (AActor* OwnerActor = GetOwner())
	{
		float ClampedTimeValue = FMath::Clamp(TimeValue, 0.0f, 1.0f);
		float ComparedFloatValue = ClampedTimeValue;
		if (SaveAlphaCurveFloatLocation != nullptr)
		{
			ComparedFloatValue = SaveAlphaCurveFloatLocation->GetFloatValue(ClampedTimeValue);
		}
		FVector NewLocation = FMath::Lerp(SaveLocation, SaveNewLocation, ComparedFloatValue);
		OwnerActor->SetActorLocation(NewLocation, bSaveSweepLocation, &SweepHitResultLocation, SaveTeleportTypeLocation);
	}
}

void UTransformComponent::HandleTimeline_Rotation(float TimeValue)
{
	if (AActor* OwnerActor = GetOwner())
	{
		float ClampedTimeValue = FMath::Clamp(TimeValue, 0.0f, 1.0f);
		float ComparedFloatValue = ClampedTimeValue;
		if (SaveAlphaCurveFloatRotation != nullptr)
		{
			ComparedFloatValue = SaveAlphaCurveFloatRotation->GetFloatValue(ClampedTimeValue);
		}
		FRotator NewRotation = FMath::Lerp(SaveRotation, SaveNewRotation, ComparedFloatValue);
		OwnerActor->SetActorRotation(NewRotation, SaveTeleportTypeRotation);
	}
}

UCurveFloat* UTransformComponent::GetAlphaCurveFloat(EAlphaCurveType AlphaCurveType)
{
	for (FAlphaCurve TempAlphaCurve : AlphaCurveArray)
	{
		if (AlphaCurveType == TempAlphaCurve.AlphaCurveType)
		{
			if (UCurveFloat* AlphaCurveFloat = UCustomAssetManager::GetObjectFromSoftPtr(TempAlphaCurve.AlphaFloatCurveSoftPtr))
			{
				return AlphaCurveFloat;
			}
		}
	}
	return nullptr;
}
