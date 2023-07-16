// Copy Right LDW. All Rights Reserved.


#include "Characters/Animation/AnimNotify_PlayCustomFX.h"
#include "Components/FXComponent.h"

void UAnimNotify_PlayCustomFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		if (UFXComponent* CustomFXComponent = Cast<UFXComponent>(MeshComp->GetOwner()->GetComponentByClass(UFXComponent::StaticClass())))
		{
			CustomFXComponent->SpawnFXAtLocation(FXId, SpawnLocation, SpawnRotation, SpawnScale, InLocalCoordinate);
		}
	}
}
