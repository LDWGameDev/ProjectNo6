// Copy Right LDW. All Rights Reserved.


#include "System/Combat/CombatComponent.h"
#include "System/Combat/CombatMove.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initializes combat moves
	for (UCombatMove* TempCombatMove : CombatMoveArray)
	{
		if (TempCombatMove)
		{
			TempCombatMove->Initialize(this);
		}
	}
}

void UCombatComponent::TriggerCombatMove(const FString& MoveID)
{
	for (UCombatMove* TempCombatMove : CombatMoveArray)
	{
		if (TempCombatMove && TempCombatMove->MoveID.Equals(MoveID))
		{
			TempCombatMove->OnMoveTriggered();

			// Broadcasts delegate for successfully triggering combat move
			if (OnTriggeredCombatMoveDelegate.IsBound())
			{
				OnTriggeredCombatMoveDelegate.Broadcast(TempCombatMove);
			}
			return;
		}
	}
}

UCombatMove* UCombatComponent::FindCombatMove(const FString& MoveID)
{
	for (UCombatMove* TempCombatMove : CombatMoveArray)
	{
		if (TempCombatMove && TempCombatMove->MoveID.Equals(MoveID))
		{
			 return TempCombatMove;
		}
	}
	return nullptr;
}

void UCombatComponent::TakeDamage(UDamageDetails* DamageDetails)
{
	if (DamageDetails)
	{	
		float TempDamageValue = float(DamageDetails->Damage);
		TempDamageValue *= (1 + TakeMoreDamageBoost / 100.0f);
		if (TempDamageValue <= 0.0f) TempDamageValue = 0.0f;
		DamageDetails->Damage = FMath::RoundToInt32(TempDamageValue);
		if (OnTookDamageDelegate.IsBound())
		{
			OnTookDamageDelegate.Broadcast(DamageDetails);
		}
	}
}

void UCombatComponent::OnCombatMoveHitTargets(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets)
{
	if (OnCombatMoveHitTargetsDelegate.IsBound())
	{
		OnCombatMoveHitTargetsDelegate.Broadcast(CombatMove, HitTargets);
	}
}
