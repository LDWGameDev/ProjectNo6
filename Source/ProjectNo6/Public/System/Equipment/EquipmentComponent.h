// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomData.h"
#include "EquipmentComponent.generated.h"

class ACharacterPlayerBase;
class UStaticMeshComponent;
class AHoldingActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentIndexChangedSignature, EHoldingObjectType, OldType, EHoldingObjectType, NewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Equipment Component")
		FOnEquipmentIndexChangedSignature OnEquipmentIndexChangedDelegate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment Component")
		FDefaultWeapon TorchDefault;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment Component")
		FDefaultWeapon SwordDefault;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment Component")
		FDefaultWeapon GunDefault;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Component")
		AHoldingActor* TorchHoldingActor;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Component")
		AHoldingActor* SwordHoldingActor;
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Component")
		AHoldingActor* GunHoldingActor;

	UPROPERTY(BlueprintReadWrite, Category = "Equipment Component")
		bool bCanChangeEquipmentIndex = false;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Component")
		EHoldingObjectType CurrentHoldingType;
	UPROPERTY()
		ACharacterPlayerBase* OwnerPlayerCharacter;

public:	
	UEquipmentComponent();

	UFUNCTION(BlueprintPure)
		EHoldingObjectType GetCurrentHoldingType() { return CurrentHoldingType; }
	UFUNCTION(BlueprintCallable)
		void SetCurrentHoldingType(EHoldingObjectType NewHoldingType, bool bShouldResetIfSameValue = false);

	UFUNCTION(BlueprintCallable)
		void UpdateCurrentHoldingType(bool bDoIncrease);

protected:
	virtual void BeginPlay() override;
	AHoldingActor* CreateHoldingActor(const FDefaultWeapon& DefaultWeapon);
};
