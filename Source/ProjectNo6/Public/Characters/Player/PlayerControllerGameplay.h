// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PlayerControllerGameplay.generated.h"

class UInputMappingContext;
class UInputAction;
class ACharacterEnemyBase;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API APlayerControllerGameplay : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** Default UInputMappingContext added to UEnhancedInputLocalPlayerSubSystem */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputMappingContext> DefaultInputMappingContextSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> MovementInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> DashInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> LightAttackInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> HeavyAttackInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> QSkillInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> MiddleMouseScrollInputActionSoftObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller Gameplay : Input")
		TSoftObjectPtr<UInputAction> EscapeInputAction;

	UPROPERTY(BlueprintReadWrite, Category = "Player Controller Gameplay")
		bool bUpdateGameplayMouseLocation = true;

	UPROPERTY(BlueprintReadWrite)
		TArray<FString> SpecialUpgradesArray;

protected:
	/** Projected world location of mouse input, with the same Z as character foot location */
	FVector GameplayMouseLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Player Controller Gameplay")
		TArray<AActor*> InteractableActors;

public:
	APlayerControllerGameplay();

	/** APlayerController override functions */
	virtual void Tick(float DeltaSeconds) override;

	/** Getters, setters */
	UFUNCTION(BlueprintPure)
		const FVector& GetGameplayMouseLocation() { return GameplayMouseLocation; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Controller Gameplay")
		void BackToPersistentGameplay();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Controller Gameplay")
		void TriggerTempleSelection();

	UFUNCTION(BlueprintCallable, Category = "Player Controller Gameplay")
		void AddInteractableActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Player Controller Gameplay")
		void RemoveInteractableActor(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller Gameplay")
		void TransitionToBlack();
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller Gameplay")
		void TransitionOutOfBlack();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Player Controller Gameplay")
		void HandleEscapeTriggered(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player Controller Gameplay")
		void HandleOnCompletedObjectiveDelegate(int32 ObjectiveIndex, int32 FloorIndex, EFloorObjectiveType ObjectiveType);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Player Controller")
		bool CheckInputKeyInInteractableActors(const FKey& Key);

private:
	/** Binded to EnhancedInputComponent to handle InputActions */
	UFUNCTION()
		void HandleInputActionTriggered_Movement(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputActionCompleted_Movement(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputActionTriggered_Dash(const FInputActionValue& Value);
	
	UFUNCTION()
		void HandleInputAction_LightAttack_Started(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_LightAttack_Triggered(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_LightAttack_Completed(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_LightAttack_Canceled(const FInputActionValue& Value);

	UFUNCTION()
		void HandleInputAction_HeavyAttack_Started(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_HeavyAttack_Triggered(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_HeavyAttack_Completed(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputAction_HeavyAttack_Canceled(const FInputActionValue& Value);

	UFUNCTION()
		void HandleInputActionTriggered_QSkill(const FInputActionValue& Value);
	UFUNCTION()
		void HandleInputActionTriggered_MiddleMouseScroll(const FInputActionValue& Value);

	/** Calculates gameplay mouse location */
	void CalculatesGameplayMouseLocation();
	void DisableDefaultFocusInput();

public:
	UPROPERTY(BlueprintReadOnly)
		int32 Coins;
public:
	UFUNCTION(BlueprintCallable)
		bool AddToCoins(int32 AmountToAdd);
protected:
	UFUNCTION(BlueprintNativeEvent)
		void OnCoinsValueChanged(int32 ChangeAmount, int32 LastValue, int32 NewValue);
	virtual void OnCoinsValueChanged_Implementation(int32 ChangeAmount, int32 LastValue, int32 NewValue);
};
