// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class UStatComponent;
class UCombatComponent;

UCLASS()
class PROJECTNO6_API ATrapBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* RootSceneComp;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//	UStatComponent* StatComp;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//	UCombatComponent* CombatComp;

public:
	ATrapBase();
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
		bool bIsActivated = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CooldownTime = 1.0f;
	UPROPERTY(BlueprintReadWrite)
		bool bIsInCooldown = false;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool ActivateTrap();
	virtual bool ActivateTrap_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DeactivateTrap();
	virtual void DeactivateTrap_Implementation();
};
