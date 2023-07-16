// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomData.h"
#include "StatBase.h"
#include "StatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "Stat Comp")
		TArray<UStatBase*> StatArray;

	UPROPERTY(BlueprintAssignable, Category = "Stat Comp")
		FOnStatValueChangedSignature OnStatValueChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Stat Comp")
		FOnStatMinMaxValueChangedSignature OnStatMinMaxValueChangedDelegate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stat Comp : Power Regenerate")
		bool bHasPowerStat;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stat Comp : Power Regenerate")
		float ActionDelayRegenerate = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat Comp : Power Regenerate")
		float RegenerateInterval = 0.25f;
	UPROPERTY(BlueprintReadWrite, Category = "Stat Comp : Power Regenerate")
		bool bShouldRegenerateStamina;
	UPROPERTY(BlueprintReadOnly, Category = "Stat Comp : Power Regenerate")
		UStatBase* PowerStat;

private:
	FTimerHandle HandleWaitRegenerate;
	FTimerHandle HandleRegenerate;

public:	
	UStatComponent();
	UFUNCTION(BlueprintPure)
		UStatBase* GetStat(ECustomStatType StatToGet);

	UFUNCTION(BlueprintCallable, Category = "Stat Comp : Power Regenerate")
		void StartDelayRegeneratePower();
	UFUNCTION(BlueprintCallable, Category = "Stat Comp : Power Regenerate")
		void StopRegeneratePower();

protected:
	/** UActorComponent override */
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleOnStatValueChanged(UStatBase* ChangedStat, int32 OldValue, int32 NewValue);
	UFUNCTION()
		void HandleOnStatMinMaxValueChanged(UStatBase* ChangedStat, int32 OldMin, int32 OldMax, int32 NewMin, int32 NewMax);
	UFUNCTION()
		void RegeneratePower();
};
