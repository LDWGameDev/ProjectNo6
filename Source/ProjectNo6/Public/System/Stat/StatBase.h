// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomData.h"
#include "StatBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatValueChangedSignature, UStatBase*, ChangedStat, int32, OldValue, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnStatMinMaxValueChangedSignature, UStatBase*, ChangedStat, int32, OldMin, int32, OldMax, int32, NewMin, int32, NewMax);

class UStatComponent;

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType)
class PROJECTNO6_API UStatBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		ECustomStatType Type = ECustomStatType::None;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		bool bIsDynamic = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		bool bIsPercentage = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		int32 MinValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		int32 MaxValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat")
		int32 BaseValue;

	UPROPERTY(BlueprintAssignable, Category = "Stat")
		FOnStatValueChangedSignature OnStatValueChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Stat")
		FOnStatMinMaxValueChangedSignature OnStatMinMaxChangedDelegate;

protected:
	int32 CurrentValue;

public:
	UStatBase();

	UFUNCTION(BlueprintNativeEvent)
		void Initialize(UStatComponent* OwnerStatComponent);
	UFUNCTION(BlueprintPure)
		virtual int32 GetValue();
	UFUNCTION(BlueprintCallable)
		bool AddValue(int32 AmountToAdd);

	/**
	 * Set CurrentValue of this stat
	 * @Param	NewValue		- Value to set to CurrentValue
	 * @Param	bClampToMinMax	- Indicates whether to clamp new value within min max range. If false and new value is not in min max range, do not set new value and return false.
	 */
	UFUNCTION(BlueprintCallable)
		bool SetCurrentValue(int32 NewValue, bool bClampToMinMax = true);

	UFUNCTION(BlueprintCallable)
		bool SetMinMaxValue(int32 NewMinValue, int32 NewMaxValue);
};