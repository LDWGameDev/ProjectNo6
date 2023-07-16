// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterPlayerBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPlayerStateMachineComponent;
class UAnimationComponent;
class UTransformComponent;
class UFXComponent;
class UCombatComponent;
class UGameplayTagComponent;
class UCombatMove;
class UStatComponent;
class APlayerControllerGameplay;
class UEquipmentComponent;
class UPointLightComponent;
class UWidgetComponent;
class USplineComponent;
class UStatBase;
class UDamageDetails;

UCLASS()
class PROJECTNO6_API ACharacterPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Player Base")
		float RotateTowardMouseSpeed = 10.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Character Player Base")
		bool bShouldRotateToMouse = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Player Base")
		float DefaultWalkSpeed = 600.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Character Player Base")
		FVector2D LastMovementInput;

	UPROPERTY(BlueprintReadOnly, Category = "Character Player Base")
		bool bIsHoldingLightAttackInput = false;
	UPROPERTY(BlueprintReadOnly, Category = "Character Player Base")
		bool bIsHoldingHeavyAttackInput = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Player Base")
		float DefaultCameraLength;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		USpringArmComponent* MainSpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UCameraComponent* MainCameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UPlayerStateMachineComponent* StateMachineComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UAnimationComponent* AnimationComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UTransformComponent* TransformComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UFXComponent* FXComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UCombatComponent* CombatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UGameplayTagComponent* TagComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UStatComponent* StatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UEquipmentComponent* EquipComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UPointLightComponent* TorchPointLightComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UWidgetComponent* UnderFootDirectionWidgetComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Player Base")
		UWidgetComponent* PersistentStaminaWidgetComp;

	int32 FlowIndex;
	UPROPERTY(BlueprintReadOnly, Category = "Character Player Base")
		bool bIsDeath = false;

	UPROPERTY(BlueprintReadWrite)
		APlayerControllerGameplay* OwnerGameplayPlayerController;

public:	
	ACharacterPlayerBase();

	/** Override ACharacter functions */
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	
	/** Getters, setters */
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UCameraComponent* GetMainCameraComp() { return MainCameraComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		USpringArmComponent* GetMainSpringArmComp() { return MainSpringArmComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UPlayerStateMachineComponent* GetStateMachineComponent() { return StateMachineComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UAnimationComponent* GetAnimationComponent() { return AnimationComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UTransformComponent* GetTransformComponent() { return TransformComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UFXComponent* GetFXComponent() { return FXComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UCombatComponent* GetCombatComp() { return CombatComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UGameplayTagComponent* GetTagComp() { return TagComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UStatComponent* GetStatComp() { return StatComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UEquipmentComponent* GetEquipmentComp() { return EquipComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UPointLightComponent* GetTorchSpotLightComp() {return TorchPointLightComp;}
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UWidgetComponent* GetUnderFootWidgetComp() { return UnderFootDirectionWidgetComp; }
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		UWidgetComponent* GetPersistentStaminaWidgetComp() { return PersistentStaminaWidgetComp; }

	/** Returns character foot location, if fail returns character location */
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		FVector GetFootLocation();
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		FVector GetGameplayMouseLocation();
	
	/** Getters, setters */
	UFUNCTION(BlueprintPure, Category = "Character Player Base")
		int32 GetFlowIndex() { return FlowIndex; }
	UFUNCTION(BlueprintCallable, Category = "Character Player Base")
		void SetFlowIndex(int32 NewIndex) { FlowIndex = NewIndex; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Player Base")
		void UnderFootDirectionEffect(float Duration = 0.4f);

	UFUNCTION(BlueprintCallable, Category = "Character Player Base")
		void CustomPlaySound(FString Id, FVector Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Player Base")
		void UpdateStamineGemsAmount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character Player Base")
		void UpdateStaminaGemState(bool bIncreased);

	UFUNCTION(BlueprintCallable, Category = "Character Player Base")
		void AddInteractableActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Character Player Base")
		void RemoveInteractableActor(AActor* Actor);

	/** Called to handle input action triggered */
	virtual void OnInputActionTriggered_Movement(const FVector2D& InputValue);
	virtual void OnInputActionCompleted_Movement(const FVector2D& InputValue);
	virtual void OnInputActionTriggered_Dash(bool InputValue);

	virtual void OnInputAction_LightAttack_Started();
	virtual void OnInputAction_LightAttack_Triggered();
	virtual void OnInputAction_LightAttack_Completed();
	virtual void OnInputAction_LightAttack_Canceled();

	virtual void OnInputAction_HeavyAttack_Started();
	virtual void OnInputAction_HeavyAttack_Triggered();
	virtual void OnInputAction_HeavyAttack_Completed();
	virtual void OnInputAction_HeavyAttack_Canceled();

	virtual void OnInputActionTriggered_HeavyAttack(bool InputValue);
	virtual void OnInputActionTriggered_QSkill(bool InputValue);
	virtual void OnInputActionTriggered_MiddleMouseScroll(float InputValue);

	/** Moves character with vector2d input */
	void MoveWithInput(const FVector2D& MovementInput);

protected: 
	virtual void BeginPlay() override;
	/** Called after a delay time after BeginPlay() */
	virtual void DelayBeginPlay();

	UFUNCTION()
		virtual void HandleDelegate_OnEquipmentIndexChanged(EHoldingObjectType OldType, EHoldingObjectType NewType);
	UFUNCTION()
		virtual void HandleDelegate_OnStatMinMaxValueChanged(UStatBase* ChangedStat, int32 OldMin, int32 OldMax, int32 NewMin, int32 NewMax);
	UFUNCTION()
		virtual void HandleDelegate_OnStatValueChanged(UStatBase* ChangedStat, int32 OldValue, int32 NewValue);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void HandleDelegate_OnTookDamage(UDamageDetails* DamageDetails);
	virtual void HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails);

	UFUNCTION(BlueprintNativeEvent)
		void OnDied();
	virtual void OnDied_Implementation();

	UFUNCTION()
		void ApplyDamageToHealthStat(UDamageDetails* DamageDetails);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void HandleDelegate_OnCombatMoveHitTargets(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets);
	virtual void HandleDelegate_OnCombatMoveHitTargets_Implementation(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets);

public:
	UFUNCTION(BlueprintCallable)
		void ForceDie();
};
