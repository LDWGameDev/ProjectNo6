// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomData.h"
#include "GameplayTagContainer.h"
#include "CombatMoveProjectile.h"
#include "ProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileTriggeredSignature, AProjectileBase*, Projectile);

class USphereComponent;
class UProjectileMovementComponent;
class UCombatMoveProjectile;
class UArrowComponent;

/**
 * Projectile actor spawned using this class will overlap pawns and block static + dynamic objects
 */
UCLASS()
class PROJECTNO6_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile : Trigger")
		EProjectileTriggerType TriggerType = EProjectileTriggerType::TriggerWhenHitOrTimeOut;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile : Trigger", meta = (EditCondition = "TriggerType == EProjectileTriggerType::TriggerWhenHit || TriggerType == EProjectileTriggerType::TriggerWhenHitOrTimeOut", EditConditionHides))
		FGameplayTagContainer OverlapTargetTag;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Projectile : Trigger", meta = (EditCondition = "TriggerType == EProjectileTriggerType::TriggerAfterDuration || TriggerType == EProjectileTriggerType::TriggerWhenHitOrTimeOut", EditConditionHides))
		float TriggerTime = 5.0f;

	/** Reference to the projectile combat move that spawns this projectile */
	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
		UCombatMoveProjectile* OwnerProjectileMove;

	UPROPERTY(BlueprintAssignable)
		FOnProjectileTriggeredSignature OnProjectileTriggeredDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		USphereComponent* RootSphereComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		UProjectileMovementComponent* ProjectileMovementComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		UCombatComponent* CombatComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Projectile")
		UArrowComponent* ArrowComp;

	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
		bool bHasTriggered = false;

public:
	/** AActor override */
	AProjectileBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
		void Init(UCombatMoveProjectile* OwnerProjectileCombatMove);

protected:
	/** AActor override */
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void HandleOnComponentHit_RootComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		virtual void HandleOnComponentBeginOverlap_RootComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
		void TriggerProjectile();

public:
	UFUNCTION(BlueprintPure)
		bool IsSelfUnreachable();
};
