// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/Combat/CombatMove.h"
#include "CombatMoveProjectile.generated.h"

class AProjectileBase;

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UCombatMoveProjectile : public UCombatMove
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile")
		TSoftClassPtr<AProjectileBase> ProjectileSoftClassPtr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile")
		bool bSpawnInLocal = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile")
		FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile")
		FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile : Debug")
		bool bDebugProjectileMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Move Projectile : Debug")
		FLinearColor ProjectileColor;

protected:
	UPROPERTY()
		AProjectileBase* ProjectileReference; 

public:
	UCombatMoveProjectile();

	/** UCombatMoveProjectile override */
	virtual void OnMoveTriggered_Implementation() override;
	virtual void DealDamageToTargets_Implementation(const TArray<AActor *>& TargetActorArray) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
		void HandleOnProjectileTriggered(AProjectileBase* Projectile);
	virtual void HandleOnProjectileTriggered_Implementation(AProjectileBase* Projectile);
};
