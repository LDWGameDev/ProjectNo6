// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayCustomFX.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Play Custom FX"))
class PROJECTNO6_API UAnimNotify_PlayCustomFX : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString FXId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator SpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool InLocalCoordinate = true;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
