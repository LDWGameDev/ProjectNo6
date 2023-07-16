// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseUpgrade.generated.h"

class APlayerController;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class PROJECTNO6_API UBaseUpgrade : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseUpgrade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
		FText Header;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
		FText MoreDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
		UTexture2D* DisplayTexture;

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		FText GetMoreDescription();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DoUpgrade(APlayerController* PlayerControllerTriggerUpgrading);
	void DoUpgrade_Implementation(APlayerController* PlayerControllerTriggerUpgrading);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ConfigUpgrade(APlayerController* PlayerControllerTriggerUpgrading);
	void ConfigUpgrade_Implementation(APlayerController* PlayerControllerTriggerUpgrading);
};
