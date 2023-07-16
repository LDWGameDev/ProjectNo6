// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNO6_API UGameplayTagComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Tag Component")
		FGameplayTagContainer OwnerTagContainer;

public:	
	UGameplayTagComponent();

	/** Getters, setters */
	UFUNCTION(BlueprintPure, Category = "Gameplay Tag Component")
		FGameplayTagContainer& GetOwnerTagContainer() { return OwnerTagContainer; }

protected:
	/** UActorComponent override functions */
	virtual void BeginPlay() override;
};
