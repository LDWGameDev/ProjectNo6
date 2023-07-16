// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoldingActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTNO6_API AHoldingActor : public AActor
{
	GENERATED_BODY()

public:	
	AHoldingActor();

	UFUNCTION(BlueprintNativeEvent, Category = "Holding Actor")
		void OnStartHolding();
	UFUNCTION(BlueprintNativeEvent, Category = "Holding Actor")
		void OnStopHolding();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Holding Actor")
		USceneComponent* RootSceneComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Holding Actor")
		UStaticMeshComponent* HoldingMeshComp;

protected:
	virtual void BeginPlay() override;
};
