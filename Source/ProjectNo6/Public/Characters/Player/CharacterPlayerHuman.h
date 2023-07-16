// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "CharacterPlayerHuman.generated.h"

class UWidgetComponent;
class UDamageDetails;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API ACharacterPlayerHuman : public ACharacterPlayerBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Character Player Human")
		UWidgetComponent* GunFireIndicatorWidgetComp;

public:
	ACharacterPlayerHuman();

	UFUNCTION(BlueprintPure, Category = "Character Player Human")
		UWidgetComponent* GetGunFireIndicatorWidgetComp() { return GunFireIndicatorWidgetComp; }

	/** Returns a boolean indicates if can dash */
	bool CheckCanDash();

protected:
	virtual void DelayBeginPlay() override;
	virtual void HandleDelegate_OnCombatMoveHitTargets_Implementation(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets) override;
	virtual void HandleDelegate_OnEquipmentIndexChanged(EHoldingObjectType OldType, EHoldingObjectType NewType) override;

protected:
	virtual void HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails);

	UFUNCTION(BlueprintNativeEvent)
		void OnEvadeAttackWithDashing(UDamageDetails* DamageDetails);
	virtual void OnEvadeAttackWithDashing_Implementation(UDamageDetails* DamageDetails);

	UFUNCTION(BlueprintNativeEvent)
		void OnGetHit(UDamageDetails* DamageDetails);
	void OnGetHit_Implementation(UDamageDetails* DamageDetails);
};
