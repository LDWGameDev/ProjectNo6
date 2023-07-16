// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_TriggerCombatMove.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Trigger Combat Move"))
class PROJECTNO6_API UAnimNotify_TriggerCombatMove : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString CombatMoveID;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
