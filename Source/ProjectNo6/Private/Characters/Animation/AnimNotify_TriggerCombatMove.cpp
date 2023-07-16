// Copy Right LDW. All Rights Reserved.


#include "Characters/Animation/AnimNotify_TriggerCombatMove.h"
#include "System/Combat/CombatComponent.h"

void UAnimNotify_TriggerCombatMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (UCombatComponent* CombatComp = Cast<UCombatComponent>(MeshComp->GetOwner()->GetComponentByClass(UCombatComponent::StaticClass())))
		{
			CombatComp->TriggerCombatMove(CombatMoveID);
		}
	}
}
