// Copy Right LDW. All Rights Reserved.

#include "Components/FXComponent.h"
#include "System/AssetManager/CustomAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Components/MovingFXHandler.h"


UFXComponent::UFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UFXComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateSplineComponents();
	CreateSparkleWidgetComp();
}

void UFXComponent::CreateSplineComponents()
{
	if (!GetOwner()) return;
	UPrimitiveComponent* OwnerRootComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (!OwnerRootComp) return;

	for (FSplineTrail& Trail : SplineTrails)
	{
		if (Trail.SplinePoints.Num() > 1)
		{
			USplineComponent* NewSplineComp = NewObject<USplineComponent>(this);
			NewSplineComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewSplineComp->AttachToComponent(OwnerRootComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			NewSplineComp->RegisterComponent();
			for (int i = 0; i < Trail.SplinePoints.Num(); i++)
			{
				if (i < NewSplineComp->GetNumberOfSplinePoints())
				{
					NewSplineComp->SetLocationAtSplinePoint(i, Trail.SplinePoints[i].Location, ESplineCoordinateSpace::Local, true);
					NewSplineComp->SetRotationAtSplinePoint(i, Trail.SplinePoints[i].Rotation, ESplineCoordinateSpace::Local, true);
					NewSplineComp->SetTangentsAtSplinePoint(i, Trail.SplinePoints[i].ArriveTangent, Trail.SplinePoints[i].LeaveTangent, ESplineCoordinateSpace::Local, true);
				}
				else
				{
					NewSplineComp->AddSplineLocalPoint(Trail.SplinePoints[i].Location);
					NewSplineComp->SetRotationAtSplinePoint(i, Trail.SplinePoints[i].Rotation, ESplineCoordinateSpace::Local, true);
					NewSplineComp->SetTangentsAtSplinePoint(i, Trail.SplinePoints[i].ArriveTangent, Trail.SplinePoints[i].LeaveTangent, ESplineCoordinateSpace::Local, true);
				}
			}
			Trail.RuntimeCreatedSplineComp = NewSplineComp;
		}
	}
}

UFXSystemComponent* UFXComponent::SpawnFXAtLocation(const FString& FXId, const FVector& SpawnLocation, const FRotator& SpawnRotation, FVector SpawnScale /*= FVector(1.0f, 1.0f, 1.0f)*/, bool bInLocalSpace /*= true*/, bool bAutoDestroy /*= true*/, EPSCPoolMethod PoolingMethod /*= EPSCPoolMethod::None*/, bool AutoActivate /*= true*/)
{
	if (!GetOwner()) return nullptr;
	for (FCustomFX& CustomFX : CustomFXArray)
	{
		// Finds the first encounter CustomFX with ID equal to FXId
		if (CustomFX.ID.Equals(FXId))
		{
			if (CustomFX.FXType == EFXType::Cascade)
			{
				// Cascade FX 
				if (UParticleSystem* CascadeSystem = UCustomAssetManager::GetObjectFromSoftPtr(CustomFX.CascadeSystemSoftObject))
				{
					// Calculates spawn transform
					FVector CalculatedSpawnLocation = bInLocalSpace ? (UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), SpawnLocation)) : SpawnLocation;
					FRotator CalculatedSpawnRotation = bInLocalSpace ? (UKismetMathLibrary::TransformRotation(GetOwner()->GetTransform(), SpawnRotation)) : SpawnRotation;

					UParticleSystemComponent* SpawnedCascadeSystemComp = UGameplayStatics::SpawnEmitterAtLocation(
						this,
						CascadeSystem,
						CalculatedSpawnLocation,
						CalculatedSpawnRotation,
						SpawnScale,
						bAutoDestroy,
						PoolingMethod,
						AutoActivate
					);
					return SpawnedCascadeSystemComp;
				}
			}
			else
			{
				// Niagara FX
				if (UNiagaraSystem* NiagaraSystem = UCustomAssetManager::GetObjectFromSoftPtr(CustomFX.NiagaraSystemSoftObject))
				{
					// Calculates spawn transform
					FVector CalculatedSpawnLocation = bInLocalSpace ? (UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), SpawnLocation)) : SpawnLocation;
					FRotator CalculatedSpawnRotation = bInLocalSpace ? (UKismetMathLibrary::TransformRotation(GetOwner()->GetTransform(), SpawnRotation)) : SpawnRotation;

					// Remap EPSCPoolMethod to ENCPoolMethod
					ENCPoolMethod NiagaraPoolMethod = ENCPoolMethod::None;
					switch (PoolingMethod)
					{
					case EPSCPoolMethod::ManualRelease:
					{
						NiagaraPoolMethod = ENCPoolMethod::ManualRelease;
						break;
					}
					case EPSCPoolMethod::ManualRelease_OnComplete:
					{
						NiagaraPoolMethod = ENCPoolMethod::ManualRelease_OnComplete;
						break;
					}
					case EPSCPoolMethod::AutoRelease:
					{
						NiagaraPoolMethod = ENCPoolMethod::AutoRelease;
						break;
					}
					case EPSCPoolMethod::FreeInPool:
					{
						NiagaraPoolMethod = ENCPoolMethod::FreeInPool;
						break;
					}
					}

					UNiagaraComponent* SpawnedNiagaraSystemComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						this,
						NiagaraSystem,
						CalculatedSpawnLocation,
						CalculatedSpawnRotation,
						SpawnScale,
						bAutoDestroy,
						AutoActivate,
						NiagaraPoolMethod,
						true
					);
					return SpawnedNiagaraSystemComp;
				}
			}
			break;
		}
	}
	return nullptr;
}

UFXSystemComponent* UFXComponent::SpawnFXAttached(const FString& FXId, const FVector& SpawnLocation, const FRotator& SpawnRotation, FVector SpawnScale /*= FVector(1.0f, 1.0f, 1.0f)*/, bool bInLocalSpace /*= true*/, USceneComponent* ComponentToAttach /*= nullptr*/, FName SocketName /*= NAME_None*/, EAttachLocation::Type AttachLocationRule /*= EAttachLocation::KeepWorldPosition*/, bool bAutoDestroy /*= true*/, EPSCPoolMethod PoolingMethod /*= EPSCPoolMethod::None*/, bool AutoActivate /*= true*/)
{
	if (!GetOwner() || !ComponentToAttach) return nullptr;
	for (FCustomFX& CustomFX : CustomFXArray)
	{
		// Finds the first encounter CustomFX with ID equal to FXId
		if (CustomFX.ID.Equals(FXId))
		{
			if (CustomFX.FXType == EFXType::Cascade)
			{
				// Cascade FX 
				if (UParticleSystem* CascadeSystem = UCustomAssetManager::GetObjectFromSoftPtr(CustomFX.CascadeSystemSoftObject))
				{
					// Calculates spawn transform
					FVector CalculatedSpawnLocation = bInLocalSpace ? (UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), SpawnLocation)) : SpawnLocation;
					FRotator CalculatedSpawnRotation = bInLocalSpace ? (UKismetMathLibrary::TransformRotation(GetOwner()->GetTransform(), SpawnRotation)) : SpawnRotation;

					UParticleSystemComponent* SpawnedCascadeSystemComp = UGameplayStatics::SpawnEmitterAttached(
						CascadeSystem,
						ComponentToAttach,
						SocketName,
						CalculatedSpawnLocation,
						CalculatedSpawnRotation,
						SpawnScale,
						AttachLocationRule,
						bAutoDestroy,
						PoolingMethod,
						AutoActivate
					);
					return SpawnedCascadeSystemComp;
				}
			}
			else
			{
				// Niagara FX
				if (UNiagaraSystem* NiagaraSystem = UCustomAssetManager::GetObjectFromSoftPtr(CustomFX.NiagaraSystemSoftObject))
				{
					// Calculates spawn transform
					FVector CalculatedSpawnLocation = bInLocalSpace ? (UKismetMathLibrary::TransformLocation(GetOwner()->GetTransform(), SpawnLocation)) : SpawnLocation;
					FRotator CalculatedSpawnRotation = bInLocalSpace ? (UKismetMathLibrary::TransformRotation(GetOwner()->GetTransform(), SpawnRotation)) : SpawnRotation;

					// Remap EPSCPoolMethod to ENCPoolMethod
					ENCPoolMethod NiagaraPoolMethod = ENCPoolMethod::None;
					switch (PoolingMethod)
					{
					case EPSCPoolMethod::ManualRelease:
					{
						NiagaraPoolMethod = ENCPoolMethod::ManualRelease;
						break;
					}
					case EPSCPoolMethod::ManualRelease_OnComplete:
					{
						NiagaraPoolMethod = ENCPoolMethod::ManualRelease_OnComplete;
						break;
					}
					case EPSCPoolMethod::AutoRelease:
					{
						NiagaraPoolMethod = ENCPoolMethod::AutoRelease;
						break;
					}
					case EPSCPoolMethod::FreeInPool:
					{
						NiagaraPoolMethod = ENCPoolMethod::FreeInPool;
						break;
					}
					}

					UNiagaraComponent* SpawnedNiagaraSystemComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
						NiagaraSystem,
						ComponentToAttach,
						NAME_None,
						CalculatedSpawnLocation,
						CalculatedSpawnRotation,
						SpawnScale,
						AttachLocationRule,
						bAutoDestroy,
						NiagaraPoolMethod,
						AutoActivate,
						true
					);
					return SpawnedNiagaraSystemComp;
				}
			}
			break;
		}
	}
	return nullptr;
}

bool UFXComponent::SpawnMovingFX(FString TrailId, FString FXId, float Duration /*= 0.5f*/)
{
	if (UCurveFloat* SplineCurveFloat = UCustomAssetManager::GetObjectFromSoftPtr<UCurveFloat>(EaseInOutFloatCurve))
	{
		for (const FSplineTrail& Trail : SplineTrails)
		{
			if (TrailId.Equals(Trail.Id) && Trail.RuntimeCreatedSplineComp)
			{
				FVector StartLocation = Trail.RuntimeCreatedSplineComp->GetLocationAtTime(0.0f, ESplineCoordinateSpace::World, false);
				UFXSystemComponent* SpawnedSystemComp = SpawnFXAtLocation(FXId, StartLocation, FRotator(), FVector(1.0f, 1.0f, 1.0f), false, false);

				UMovingFXHandler* MovingFXHandler = NewObject<UMovingFXHandler>(this);
				if (MovingFXHandler)
				{
					MovingFXHandler->StartMovingWithSpline(GetOwner(), SpawnedSystemComp, Trail.RuntimeCreatedSplineComp, SplineCurveFloat, Duration, SplineMovingInterval);
					return true;
				}
			}
		}
	}	
	return false;
}

void UFXComponent::CreateSparkleWidgetComp()
{
	TSubclassOf<UUserWidget> SparkleWidgetClass = UCustomAssetManager::GetClassFromSoftPtr<UUserWidget>(SparkleWidget);
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (!OwnerCharacter->GetMesh() || !SparkleWidgetClass) return;
		SparkleWidgetComp = NewObject<UWidgetComponent>(GetOwner());
		SparkleWidgetComp->SetupAttachment(OwnerCharacter->GetMesh());
		SparkleWidgetComp->RegisterComponent();

		UUserWidget* SparkleWidgetObject = CreateWidget<UUserWidget>(GetWorld(), SparkleWidgetClass);
		SparkleWidgetComp->SetWidget(SparkleWidgetObject);
		SparkleWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		SparkleWidgetComp->SetDrawAtDesiredSize(true);
		SparkleWidgetComp->SetVisibility(false);
	}
}
