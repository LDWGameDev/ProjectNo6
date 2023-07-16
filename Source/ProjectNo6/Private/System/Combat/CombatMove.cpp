// Copy Right LDW. All Rights Reserved.


#include "System/Combat/CombatMove.h"
#include "System/Combat/DamageDetails.h"
#include "System/Combat/CombatComponent.h"
#include "System/Stat/StatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TransformComponent.h"
#include "CustomFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatMove::UCombatMove()
{

}

void UCombatMove::Initialize_Implementation(UCombatComponent* OwnerCombatComponent)
{
	OwnerCombatComp = OwnerCombatComponent;
	if (OwnerCombatComponent && OwnerCombatComponent->GetOwner())
	{
		StatCompToGetFrom = Cast<UStatComponent>(OwnerCombatComponent->GetOwner()->GetComponentByClass(UStatComponent::StaticClass()));
	}
}

void UCombatMove::OnMoveTriggered_Implementation()
{
	
}

void UCombatMove::DealDamageToTargets_Implementation(const TArray<AActor*>& TargetActorArray)
{
	if (!OwnerCombatComp || !OwnerCombatComp->GetOwner() || TargetActorArray.Num() == 0) return;
	for (AActor* TargetActor : TargetActorArray)
	{
		if (!TargetActor) continue;
		if (UCombatComponent* TargetCombatComp = Cast<UCombatComponent>(TargetActor->GetComponentByClass(UCombatComponent::StaticClass())))
		{
			int32 MoveAttackFlatValue = 0;
			int32 MoveAttackPercentValue = 0;

			for (const FMoveStat& MoveStat : MoveStatArray)
			{
				if (MoveStat.Value <= 0) continue;
				// Get stat from StatComponent reference
				if (MoveStat.bGetFromStatComp)
				{
					if (MoveStat.bIsPercentage && StatCompToGetFrom)
					{
						if (UStatBase* StatRef = StatCompToGetFrom->GetStat(MoveStat.StatType))
						{
							if (MoveStat.StatType == ECustomStatType::ATK) MoveAttackFlatValue += FMath::RoundToInt32(StatRef->GetValue() * MoveStat.Value / 100.0f);
						}
					}
				}
				// Percentage value
				else if (MoveStat.bIsPercentage)
				{
					if (MoveStat.StatType == ECustomStatType::ATK) MoveAttackPercentValue += MoveStat.Value;
				}
				// Flat value
				else
				{
					if (MoveStat.StatType == ECustomStatType::ATK) MoveAttackFlatValue += MoveStat.Value;
				}
			}
			if (MoveAttackPercentValue > 0) MoveAttackFlatValue += FMath::RoundToInt32(MoveAttackFlatValue * MoveAttackPercentValue / 100.0f);

			// Calculate with target's defense stats
			if (UStatComponent* TargetStatComp = Cast<UStatComponent>(TargetActor->GetComponentByClass(UStatComponent::StaticClass())))
			{
				UStatBase* PhyDef = TargetStatComp->GetStat(ECustomStatType::DEF);
				if (PhyDef) MoveAttackFlatValue = (MoveAttackFlatValue - PhyDef->GetValue() > 0) ? (MoveAttackFlatValue - PhyDef->GetValue()) : 0;
			}
			MoveAttackFlatValue = FMath::RoundToInt32(float(MoveAttackFlatValue) * (1.0f + OwnerCombatComp->DealMoreDamageBoost / 100.0f));

			UDamageDetails* DamageDetails = NewObject<UDamageDetails>(OwnerCombatComp, UDamageDetails::StaticClass());
			if (DamageDetails)
			{
				DamageDetails->InstigatorActor = OwnerCombatComp->GetOwner();
				DamageDetails->InstigatorCombatMove = this;
				DamageDetails->TargetActor = TargetActor;
				DamageDetails->Damage = MoveAttackFlatValue + FMath::RandRange(0, 3);
			}
			TargetCombatComp->TakeDamage(DamageDetails);
		}
	}

	// Calls handling function from CombatComponent
	OwnerCombatComp->OnCombatMoveHitTargets(this, TargetActorArray);
}

//void UCombatMove::CircleDistributionSetWorldLocation(const FVector& WorldLocation)
//{
//	if (TargetLocationControlType == ETargetLocationControlType::CircleDistribution)
//	{
//		bCircleDistributionInLocal = false;
//		CircleDistributionCenterLocation = WorldLocation;
//	}
//}
//
//void UCombatMove::AddForceWithDirectionSetWorldDirection(const FVector& WorldDirection)
//{
//	if (TargetLocationControlType == ETargetLocationControlType::DirectionalForceAngleDistribution)
//	{
//		bDFADInLocal = false;
//		DFADDirection = WorldDirection;
//		DFADDirection.Normalize();
//	}
//}
//
//void UCombatMove::ControlLocation(TArray<AActor*>& ActorArray)
//{
//	if (ActorArray.Num() > 0)
//	{
//		switch (TargetLocationControlType)
//		{
//		case ETargetLocationControlType::CircleDistribution:
//		{
//			ControlLocationCircleDistribution(ActorArray);
//			break;
//		}
//		case ETargetLocationControlType::DirectionalForceAngleDistribution:
//		{
//			ControlLocationDirectionalForceAngleDistribution(ActorArray);
//			break;
//		}
//		case ETargetLocationControlType::RadialForce:
//		{
//			ControlLocationRadialForce(ActorArray);
//			break;
//		}
//		case ETargetLocationControlType::RadialDistribution:
//		{
//			ControlLocationRadialDistribution(ActorArray);
//			break;
//		}
//		}
//	}
//}
//
//void UCombatMove::ControlRotation(TArray<AActor*>& ActorArray)
//{
//	if (ActorArray.Num() > 0)
//	{
//		switch (TargetRotationControlType)
//		{
//		case ETargetRotationControlType::LookAtInstigator:
//		{
//			for (AActor* TargetActor : ActorArray)
//			{
//				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TargetActor->GetActorLocation(), OwnerCombatComp->GetOwner()->GetActorLocation());
//				LookAtRotation.Pitch = 0.0f;
//				LookAtRotation.Roll = 0.0f;
//
//				if (UTransformComponent* TargetTransformComp = Cast<UTransformComponent>(TargetActor->GetComponentByClass(UTransformComponent::StaticClass())))
//				{
//					TargetTransformComp->SetOwnerActorRotation(LookAtRotation, true);
//				}
//				else
//				{
//					TargetActor->SetActorRotation(LookAtRotation);
//				}
//			}
//			break;
//		}
//		}
//	}
//}
//
//void UCombatMove::ControlLocationCircleDistribution(TArray<AActor*>& ActorArray)
//{
//	FVector CircleCenterLocation = bCircleDistributionInLocal ? (UKismetMathLibrary::TransformLocation(OwnerCombatComp->GetOwner()->GetActorTransform(), CircleDistributionCenterLocation)) : CircleDistributionCenterLocation;
//	TArray<FVector2D> TargetDistributedLocationArray;
//
//	if (ActorArray.Num() == 1)
//	{
//		TargetDistributedLocationArray.Add(FVector2D(0.0f, 0.0f));
//	}
//	else
//	{
//		UCustomFunctionLibrary::SunFlowerSeedArrangement(TargetDistributedLocationArray, ActorArray.Num(), CircleDistributionBoundarySmooth);
//	}
//
//	if (bDebugCombatMove)
//	{
//		UKismetSystemLibrary::DrawDebugCircle(
//			this,
//			CircleCenterLocation,
//			CircleDistributionRadius,
//			24,
//			CombatMoveDebugColor,
//			CombatMoveDebugTime,
//			2.0f,
//			FVector::RightVector,
//			FVector::ForwardVector
//		);
//	}
//
//	if (TargetDistributedLocationArray.Num() == ActorArray.Num())
//	{
//		// Sort location array and target array
//		/*TargetDistributedLocationArray.Sort([&](const FVector2D& Value1, const FVector2D& Value2){
//
//		});*/
//
//		for (int i = 0; i < TargetDistributedLocationArray.Num(); i++)
//		{
//			if (!ActorArray[i]) continue;
//			float TargetX = CircleCenterLocation.X + CircleDistributionRadius * TargetDistributedLocationArray[i].X;
//			float TargetY = CircleCenterLocation.Y + CircleDistributionRadius * TargetDistributedLocationArray[i].Y;
//			float TargetZ = ActorArray[i]->GetActorLocation().Z;
//			FVector TargetLocation = FVector(TargetX, TargetY, TargetZ);
//
//			if (UTransformComponent* TargetTransformComp = Cast<UTransformComponent>(ActorArray[i]->GetComponentByClass(UTransformComponent::StaticClass())))
//			{
//				TargetTransformComp->SetOwnerActorLocation(TargetLocation, true);
//			}
//			else
//			{
//				ActorArray[i]->SetActorLocation(TargetLocation);
//			}
//
//			// Debug distributed location
//			if (bDebugCombatMove)
//			{
//				UKismetSystemLibrary::DrawDebugSphere(
//					this,
//					TargetLocation,
//					CircleDistributionDebugRadius,
//					8,
//					CombatMoveDebugColor,
//					CombatMoveDebugTime,
//					1.0f
//				);
//			}
//		}
//	}
//}
//
//void UCombatMove::ControlLocationDirectionalForceAngleDistribution(TArray<AActor*>& ActorArray)
//{
//	// Array of force direction used to launch characters
//	TArray<FVector> TargetForceDirectionArray;
//	TargetForceDirectionArray.Reserve(ActorArray.Num());
//	FVector ForceDirection = (bDFADInLocal) ? UKismetMathLibrary::TransformDirection(OwnerCombatComp->GetOwner()->GetActorTransform(), DFADDirection) : DFADDirection;
//	ForceDirection.Normalize();
//
//	if (ActorArray.Num() == 1)
//	{
//		// Only one target, use default force direction
//		TargetForceDirectionArray.Add(ForceDirection);
//	}
//	else
//	{
//		// Check to make sure angle between targets do not exceed a specified value (in this case is DFADMaxAngleBetweenTargets)
//		// Number of targets always > 1
//		float DistributeMaxAngle = ((ActorArray.Num() - 1) * DFADMaxAngleBetweenTargets < DFADMaxAngle) ? ((ActorArray.Num() - 1) * DFADMaxAngleBetweenTargets) : DFADMaxAngle;
//		float AngleBetweenTargets = DistributeMaxAngle / (ActorArray.Num() - 1);
//
//		// Farthest left direction
//		float LeftBoundDirectionAngle = (ActorArray.Num() % 2 == 0) ? (AngleBetweenTargets / 2 + (ActorArray.Num() / 2 - 1) * AngleBetweenTargets) : ((ActorArray.Num() / 2) * AngleBetweenTargets);
//		FVector LeftBoundDirection = ForceDirection.RotateAngleAxis(-LeftBoundDirectionAngle, FVector::UpVector);
//		TargetForceDirectionArray.Add(LeftBoundDirection);
//
//		// Rotate based on left bound direction
//		for (int i = 1; i < ActorArray.Num(); i++)
//		{
//			FVector TargetDirection = TargetForceDirectionArray[0].RotateAngleAxis(AngleBetweenTargets * i, FVector::UpVector);
//			TargetForceDirectionArray.Add(TargetDirection);
//		}
//	}
//
//	if (bDebugCombatMove)
//	{
//		for (int i = 0; i < ActorArray.Num(); i++)
//		{
//			if (!ActorArray[i]) continue;
//			UKismetSystemLibrary::DrawDebugArrow(
//				this,
//				ActorArray[i]->GetActorLocation(),
//				ActorArray[i]->GetActorLocation() + TargetForceDirectionArray[i] * 300.0f,
//				3.0f,
//				CombatMoveDebugColor,
//				CombatMoveDebugTime,
//				3.0f
//			);
//		}
//	}
//
//	for (int i = 0; i < ActorArray.Num(); i++)
//	{
//		if (!ActorArray[i]) continue;
//
//		if (UTransformComponent* TargetTransformComponent = Cast<UTransformComponent>(ActorArray[i]->GetComponentByClass(UTransformComponent::StaticClass())))
//		{
//			TargetTransformComponent->StopControllingTransform(true, false);
//		}
//
//		// Add force
//		if (UCharacterMovementComponent* TargetCharacterMovementComp = Cast<UCharacterMovementComponent>(ActorArray[i]->GetComponentByClass(UCharacterMovementComponent::StaticClass())))
//		{
//			TargetCharacterMovementComp->StopMovementImmediately();
//			FVector Force = TargetForceDirectionArray[i] * FMath::FRandRange(DFADStrength.X, DFADStrength.Y);
//			TargetCharacterMovementComp->AddImpulse(Force, true);
//		}
//		else if (UPrimitiveComponent* TargetRootPrimitiveComp = Cast<UPrimitiveComponent>(ActorArray[i]->GetComponentByClass(UPrimitiveComponent::StaticClass())))
//		{
//			if (TargetRootPrimitiveComp->IsSimulatingPhysics())
//			{
//				FVector Force = TargetForceDirectionArray[i] * FMath::FRandRange(DFADStrength.X, DFADStrength.Y);
//				TargetRootPrimitiveComp->AddImpulse(Force, NAME_None, true);
//			}
//		}
//	}
//}
//
//void UCombatMove::ControlLocationRadialDistribution(TArray<AActor*>& ActorArray)
//{
//	if (!OwnerCombatComp || !OwnerCombatComp->GetOwner()) return;
//	FVector CenterLocation = (bRadialDistributionInLocal) ? (OwnerCombatComp->GetOwner()->GetActorTransform().TransformPosition(RadialDistributionLocation)) : RadialDistributionLocation;
//	
//	for (AActor* TargetActor : ActorArray)
//	{
//		if (!TargetActor) continue;
//		FVector ForceDirection = TargetActor->GetActorLocation() - CenterLocation;
//		ForceDirection.Z = 0.0f;
//		ForceDirection.Normalize(0.0001f);
//		FRotator ForceDirectionRotation = UKismetMathLibrary::MakeRotFromXZ(ForceDirection, FVector::UpVector);
//
//		FVector DistributedLocation = ForceDirection * RadialDistributionRadius + CenterLocation;
//		DistributedLocation = DistributedLocation + FMath::RandRange(-RadialDistributionVariation.X, RadialDistributionVariation.X) * ForceDirection + 
//			FMath::RandRange(-RadialDistributionVariation.Y, RadialDistributionVariation.Y) * UKismetMathLibrary::GetRightVector(ForceDirectionRotation);
//		DistributedLocation.Z = TargetActor->GetActorLocation().Z;
//
//		if ((DistributedLocation - CenterLocation).Length() > (TargetActor->GetActorLocation() - CenterLocation).Length())
//		{
//			if (UTransformComponent* TargetTransform = Cast<UTransformComponent>(TargetActor->GetComponentByClass(UTransformComponent::StaticClass())))
//			{
//				TargetTransform->SetOwnerActorLocation(DistributedLocation, true, true);
//			}
//			else
//			{
//				TargetActor->SetActorLocation(DistributedLocation, true);
//			}
//		}
//
//		if (bDebugCombatMove)
//		{
//			UKismetSystemLibrary::DrawDebugSphere(
//				this,
//				DistributedLocation,
//				5.0f, 
//				12, 
//				CombatMoveDebugColor,
//				CombatMoveDebugTime,
//				1.0f
//			);
//		}
//	}
//
//	if (bDebugCombatMove)
//	{
//		UKismetSystemLibrary::DrawDebugCircle(
//			this,
//			CenterLocation,
//			RadialDistributionRadius,
//			24,
//			CombatMoveDebugColor,
//			CombatMoveDebugTime,
//			2.0f,
//			FVector::RightVector,
//			FVector::ForwardVector
//		);
//	}
//}
//
//void UCombatMove::ControlLocationRadialForce(TArray<AActor*> ActorArray)
//{
//	if (!OwnerCombatComp || !OwnerCombatComp->GetOwner()) return;
//	FVector CenterLocation = (bRadialForceInLocal) ? (OwnerCombatComp->GetOwner()->GetActorTransform().TransformPosition(RadialForceLocation)) : RadialForceLocation;
//
//	for (AActor* TargetActor : ActorArray)
//	{
//		if (!TargetActor) continue;
//
//		if (UTransformComponent* TargetTransformComponent = Cast<UTransformComponent>(TargetActor->GetComponentByClass(UTransformComponent::StaticClass())))
//		{
//			TargetTransformComponent->StopControllingTransform(true, false);
//		}
//
//		// Add force
//		FVector ForceDirection = TargetActor->GetActorLocation() - CenterLocation;
//		ForceDirection.Z = 0.0f;
//		ForceDirection.Normalize();
//		ForceDirection = ForceDirection.RotateAngleAxis(-RadialForceAngle, FRotationMatrix(FRotationMatrix::MakeFromXZ(ForceDirection, FVector::UpVector).Rotator()).GetScaledAxis(EAxis::Y));
//		FVector Force = ForceDirection * FMath::RandRange(RadialForceStrength.X, RadialForceStrength.Y);
//		if (UCharacterMovementComponent* TargetCharacterMovementComp = Cast<UCharacterMovementComponent>(TargetActor->GetComponentByClass(UCharacterMovementComponent::StaticClass())))
//		{
//			TargetCharacterMovementComp->StopMovementImmediately();
//			TargetCharacterMovementComp->AddImpulse(Force, true);
//		}
//		else if (UPrimitiveComponent* TargetRootPrimitiveComp = Cast<UPrimitiveComponent>(TargetActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
//		{
//			if (TargetRootPrimitiveComp->IsSimulatingPhysics())
//			{
//				TargetRootPrimitiveComp->AddImpulse(Force, NAME_None, true);
//			}
//		}
//
//		if (bDebugCombatMove)
//		{
//			UKismetSystemLibrary::DrawDebugArrow(
//				this, 
//				CenterLocation, 
//				CenterLocation + ForceDirection * 200.0f,
//				10.0f, 
//				CombatMoveDebugColor, 
//				CombatMoveDebugTime, 
//				5.0f
//			);
//		}
//	}
//}
