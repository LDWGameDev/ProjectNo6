// Copy Right LDW. All Rights Reserved.


#include "AnimNotify_StateActionString.h"
#include "LDWStateMachineComponent.h"
#include "LDWBaseState.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_StateActionString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ActionID.IsEmpty()) return;
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ULDWStateMachineComponent* StateMachineComp = Cast<ULDWStateMachineComponent>(MeshComp->GetOwner()->GetComponentByClass(ULDWStateMachineComponent::StaticClass())))
		{
			if (StateMachineComp->CheckIsActivated() && StateMachineComp->GetCurrentState())
			{
				StateMachineComp->GetCurrentState()->HandleActionString(ActionID);
			}
		}
	}
}
