// Copy Right LDW. All Rights Reserved.


#include "System/Equipment/EquipmentComponent.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "System/Equipment/HoldingActor.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayerCharacter = Cast<ACharacterPlayerBase>(GetOwner());
	TorchHoldingActor = CreateHoldingActor(TorchDefault);
	SwordHoldingActor = CreateHoldingActor(SwordDefault);
	GunHoldingActor = CreateHoldingActor(GunDefault);
}

void UEquipmentComponent::SetCurrentHoldingType(EHoldingObjectType NewHoldingType, bool bShouldResetIfSameValue /*= false*/)
{
	if (!bCanChangeEquipmentIndex) return;

	if (!bShouldResetIfSameValue && NewHoldingType == CurrentHoldingType) return;
	if (TorchHoldingActor && SwordHoldingActor && GunHoldingActor)
	{
		switch (NewHoldingType)
		{
			case EHoldingObjectType::None:
			{
				GunHoldingActor->OnStopHolding();
				SwordHoldingActor->OnStopHolding();
				TorchHoldingActor->OnStopHolding();
				break;
			}
			case EHoldingObjectType::Torch: 
			{
				GunHoldingActor->OnStopHolding();
				SwordHoldingActor->OnStopHolding();
				TorchHoldingActor->OnStartHolding();
				break;
			}
			case EHoldingObjectType::GunSword:
			{
				TorchHoldingActor->OnStopHolding();
				GunHoldingActor->OnStartHolding();
				SwordHoldingActor->OnStartHolding();
				break;
			}
		}
	}
	EHoldingObjectType OldType = CurrentHoldingType;
	CurrentHoldingType = NewHoldingType;
	if (OnEquipmentIndexChangedDelegate.IsBound())
	{
		OnEquipmentIndexChangedDelegate.Broadcast(OldType, NewHoldingType);
	}
}

void UEquipmentComponent::UpdateCurrentHoldingType(bool bDoIncrease)
{
	SetCurrentHoldingType((CurrentHoldingType == EHoldingObjectType::Torch) ? EHoldingObjectType::GunSword : EHoldingObjectType::Torch);
}

AHoldingActor* UEquipmentComponent::CreateHoldingActor(const FDefaultWeapon& DefaultWeapon)
{
	if (!OwnerPlayerCharacter) return nullptr;
	if (DefaultWeapon.HoldingActorClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		AHoldingActor* HoldingRef = GetWorld()->SpawnActor<AHoldingActor>(DefaultWeapon.HoldingActorClass, SpawnParams);
		if (HoldingRef && OwnerPlayerCharacter->GetMesh()->DoesSocketExist(DefaultWeapon.SocketAttachTo))
		{
			FTransform SocketTransform = OwnerPlayerCharacter->GetMesh()->GetSocketTransform(DefaultWeapon.SocketAttachTo);
			HoldingRef->SetActorLocation(SocketTransform.TransformPosition(DefaultWeapon.OffsetLocation));
			HoldingRef->SetActorRotation(SocketTransform.TransformRotation(DefaultWeapon.OffsetRotation.Quaternion()));
			HoldingRef->AttachToComponent(OwnerPlayerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, DefaultWeapon.SocketAttachTo);
		}
		return HoldingRef;
	}
	return nullptr;
}
