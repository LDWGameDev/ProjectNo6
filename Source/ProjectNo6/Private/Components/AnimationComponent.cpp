// Copy Right LDW. All Rights Reserved.


#include "Components/AnimationComponent.h"
#include "System/AssetManager/CustomAssetManager.h"
#include "GameFramework/Character.h"


UAnimationComponent::UAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter* OwnerCharacter = GetOwner<ACharacter>())
	{
		if (OwnerCharacter->GetMesh())
		{
			OwnerAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		}
	}
	LastPlayedCustomMontage.Priority = -1000;
}

float UAnimationComponent::PlayCustomMontage(const FString& MontageID, const FName& MontageSection, float PlayRate /*= 1.0f*/)
{
	if (!OwnerAnimInstance) return 0.0f;
	for (FCustomMontage& TempMontage : CustomMontageList)
	{
		if (MontageID.Equals(TempMontage.MontageID) && CheckCanPlayCustomMontage(TempMontage))
		{
			UAnimMontage* MontageToPlay = UCustomAssetManager::GetObjectFromSoftPtr<UAnimMontage>(TempMontage.AnimMontageSoftObject);
			ACharacter* OwnerCharacter = GetOwner<ACharacter>();
			if (MontageToPlay && OwnerCharacter)
			{
				LastPlayedCustomMontage = TempMontage;
				return OwnerCharacter->PlayAnimMontage(MontageToPlay, PlayRate, MontageSection);
			}
		}
	}
	return 0.0f;
}

bool UAnimationComponent::CheckCanPlayCustomMontage(const FCustomMontage& CustomMontageToPlay)
{
	if (!OwnerAnimInstance) return false;
	
	// Can play custom montage if no montage is currently playing 
	if (!OwnerAnimInstance->IsAnyMontagePlaying()) return true;
	
	// Check for priority. If same value, check if can play with same priority
	return ((LastPlayedCustomMontage.Priority < CustomMontageToPlay.Priority) || ((LastPlayedCustomMontage.Priority == CustomMontageToPlay.Priority) && (CustomMontageToPlay.bCanPlayIfSamePriority)));
}