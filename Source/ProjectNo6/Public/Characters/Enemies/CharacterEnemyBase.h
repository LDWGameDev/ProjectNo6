// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterEnemyBase.generated.h"

class UAnimationComponent;
class UTransformComponent;
class UFXComponent;
class UEnemyStateMachineComponent;
class UGameplayTagComponent;
class UCombatComponent;
class UStatComponent;
class UDamageDetails;
class UWidgetComponent;
class UBTNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneEnemyDiedSignature, ACharacterEnemyBase*, DeathEnemy);

/**
 * 
 */
UCLASS()
class PROJECTNO6_API ACharacterEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Character Enemey Base")
		bool bIsDeath = false;
	UPROPERTY(BlueprintAssignable, Category = "Character Enemey Base")
		FOneEnemyDiedSignature OnDiedDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UAnimationComponent* AnimationComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UTransformComponent* TransformComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UFXComponent* FXComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UEnemyStateMachineComponent* StateMachineComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UGameplayTagComponent* TagComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UCombatComponent* CombatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Enemy Base")
		UStatComponent* StatComp;

	FTimerHandle MaterialAuraOutlineTimerHandle;

public:	
	ACharacterEnemyBase();

	UFUNCTION(BlueprintPure)
		UCombatComponent* GetCombatComp() { return CombatComp; }

	/** ACharacter override functions */
	virtual void Tick(float DeltaTime) override;

protected:
	/** ACharacter override functions */
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void HandleDelegate_OnTriggeredCombatMove(UCombatMove* CombatMove);

	UFUNCTION(BlueprintCallable)
		void SetScalarParameterOnMaterials(FName ParaName, float ScalarValue);
	UFUNCTION(BlueprintCallable)
		void SetVectorParameterOnMaterials(FName ParaName, FVector VectorValue);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void HandleDelegate_OnTookDamage(UDamageDetails* DamageDetails);
	virtual void HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails);

	UFUNCTION(BlueprintCallable)
		void StepBackWhenGetHit(UDamageDetails* DamageDetails, float StepBackDistance = 150.0f);
	UFUNCTION(BlueprintCallable)
		void RotateToFaceInstigatorWhenGetHit(UDamageDetails* DamageDetails);
	UFUNCTION(BlueprintCallable)
		void ReduceHealthWhenGetHit(UDamageDetails* DamageDetails);
	UFUNCTION(BlueprintCallable)
		void PlayGetHitOutlineEffect(float OutlinePower = 6.0f);

	UFUNCTION(BlueprintNativeEvent)
		void OnDied();
	virtual void OnDied_Implementation();
	UFUNCTION(BlueprintCallable)
		void OnDiedCode();

protected:
	UFUNCTION()
		void HandleDelegate_OnCombatMoveHitTarget(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets);

public:
	UFUNCTION(BlueprintPure)
		const UBTNode* GetCurrentActiveBTNode();

	UFUNCTION(BlueprintCallable)
		void ForceOnDied();
};
