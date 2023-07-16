// Copy Right LDW. All Rights Reserved.

#include "System/Combat/CombatMoveSphereOverlap.h"
#include "System/Combat/CombatComponent.h"
#include "System/AssetManager/CustomAssetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/GameplayTagComponent.h"


UCombatMoveSphereOverlap::UCombatMoveSphereOverlap()
{
	bInLocalCoordinate = true;
	ActorClassFilterSoftClass = AActor::StaticClass();
}

void UCombatMoveSphereOverlap::OnMoveTriggered_Implementation()
{
	Super::OnMoveTriggered_Implementation();
	if (!OwnerCombatComp || !OwnerCombatComp->GetOwner()) return;

	// Checks for targets using sphere overlap actors
	TArray<AActor*> OverlappedActors;
	TArray<AActor*> IgnoreActors;

	// Gets world location and actor class filter
	FVector WorldOverlapLocation = bInLocalCoordinate ? (UKismetMathLibrary::TransformLocation(OwnerCombatComp->GetOwner()->GetActorTransform(), OverlapLocation)) : OverlapLocation;
	TSubclassOf ActorClassFilter = UCustomAssetManager::GetClassFromSoftPtr<AActor>(ActorClassFilterSoftClass);
	if (!ActorClassFilter) ActorClassFilter = AActor::StaticClass();

	bool Result = UKismetSystemLibrary::SphereOverlapActors(
		OwnerCombatComp->GetOwner(),
		WorldOverlapLocation, 
		SphereRadius, 
		ObjectTypes,
		ActorClassFilter,
		IgnoreActors,
		OverlappedActors
	);

	// Actors that have UGameplayTagContainer and satisfy target tag container of combat move
	TArray<AActor*> QualifiedActors;
	if (Result)
	{
		for (AActor* CheckingActor : OverlappedActors)
		{
			if (CheckingActor)
			{
				if (UGameplayTagComponent* CheckingActorTagComp = Cast<UGameplayTagComponent>(CheckingActor->GetComponentByClass(UGameplayTagComponent::StaticClass())))
				{
					if (CheckingActorTagComp->GetOwnerTagContainer().HasAny(TargetTagContainer))
					{
						QualifiedActors.Add(CheckingActor);
					}
				}
			}
		}
	}

	// Debugs overlap sphere
	if (bDebugSphereOverlap)
	{
		UKismetSystemLibrary::DrawDebugSphere(
			this, 
			WorldOverlapLocation,
			SphereRadius,
			8, 
			(QualifiedActors.Num() > 0) ? HitColor : NotHitColor,
			SphereOverlapDebugTime,
			1.0f
		);

		UE_LOG(LogTemp, Warning, TEXT("CombatMoveSphereOverlap - Overlapped actors num: %d, qualified actors num: %d"), OverlappedActors.Num(), QualifiedActors.Num());
	}

	// Deals damage to overlapped actors
	if (QualifiedActors.Num() > 0)
	{
		DealDamageToTargets(QualifiedActors);
	}
}

void UCombatMoveSphereOverlap::DealDamageToTargets_Implementation(const TArray<AActor*>& TargetActorArray)
{
	// Handles others in here
	Super::DealDamageToTargets_Implementation(TargetActorArray);
}

void UCombatMoveSphereOverlap::SphereOverlapSetWorldLocation(const FVector& WorldLocation)
{
	bInLocalCoordinate = false;
	OverlapLocation = WorldLocation;
}
