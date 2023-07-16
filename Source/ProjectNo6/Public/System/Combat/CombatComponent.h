// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggeredCombatMove, UCombatMove*, CombatMoveTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTookDamage, UDamageDetails*, DamageDetailsTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatMoveHitTargets, UCombatMove*, CombatMove, const TArray<AActor*>&, HitTargets);

class UCombatMove;
class UDamageDetails;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Delegate that is broad casted when successfully triggered a combat move  */
	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
		FOnTriggeredCombatMove OnTriggeredCombatMoveDelegate;
	/** Delegate that is broad casted when took damage (received DamageDetails) */
	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
		FOnTookDamage OnTookDamageDelegate;
	/** Delegate that is broad casted when a combat move is successfully triggered and hit any targets */
	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
		FOnCombatMoveHitTargets OnCombatMoveHitTargetsDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Component")
		int32 GetHitPowerRegenerate = 6;
	UPROPERTY(BlueprintReadWrite)
		UDamageDetails* LastDamageDetails;

protected:
	UPROPERTY(Instanced, BlueprintReadOnly, EditAnywhere, Category = "Combat Component")
		TArray<UCombatMove*> CombatMoveArray;

public:	
	UCombatComponent();

	/** Finds and triggers combat move if valid */
	UFUNCTION(BlueprintCallable, Category = "Combat Move")
		void TriggerCombatMove(const FString& MoveID);
	
	/** Returns combat move has same ID if found */
	UFUNCTION(BlueprintPure, Category = "Combat Move")
		UCombatMove* FindCombatMove(const FString& MoveID);

	/** Takes UDamageDetails created by CombatMove */
	UFUNCTION(BlueprintCallable, Category = "Combat Move")
		void TakeDamage(UDamageDetails* DamageDetails);
	/** Called when a CombatMove was successfully triggered and hit targets */
	UFUNCTION(BlueprintCallable, Category = "Combat Move")
		void OnCombatMoveHitTargets(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
		float DealMoreDamageBoost = 0.0f;
	UPROPERTY(BlueprintReadWrite)
		float TakeMoreDamageBoost = 0.0f;
};
