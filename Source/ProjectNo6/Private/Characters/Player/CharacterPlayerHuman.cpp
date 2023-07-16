// Copy Right LDW. All Rights Reserved.


#include "Characters/Player/CharacterPlayerHuman.h"
#include "Components/CapsuleComponent.h"
#include "Components/AnimationComponent.h"
#include "System/Combat/CombatMove.h"
#include "System/StateMachine/Player/PlayerStateMachineComponent.h"
#include "Components/FXComponent.h"
#include "Components/WidgetComponent.h"
#include "System/StateMachine/Player/PlayerBaseState.h"
#include "GameInstanceCustom.h"
#include "System/Stat/StatComponent.h"

ACharacterPlayerHuman::ACharacterPlayerHuman()
{
	GunFireIndicatorWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("GunFireIndicatorWidgetComp"));
	GunFireIndicatorWidgetComp->SetupAttachment(RootComponent);

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->InitCapsuleSize(60.0f, 100.0f);
	}

	if (USkeletalMeshComponent* SkeleMeshComp = GetMesh())
	{
		SkeleMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	}

	DefaultCameraLength = 2200.0f;
}

void ACharacterPlayerHuman::DelayBeginPlay()
{
	Super::DelayBeginPlay();

	if (StateMachineComp)
	{
		StateMachineComp->ActivateStateMachine(TEXT("player_human_locomotion_state"));
	}
}

bool ACharacterPlayerHuman::CheckCanDash()
{
	return true;
}

void ACharacterPlayerHuman::HandleDelegate_OnCombatMoveHitTargets_Implementation(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets)
{
	Super::HandleDelegate_OnCombatMoveHitTargets_Implementation(CombatMove, HitTargets);
}

void ACharacterPlayerHuman::HandleDelegate_OnEquipmentIndexChanged(EHoldingObjectType OldType, EHoldingObjectType NewType)
{
	Super::HandleDelegate_OnEquipmentIndexChanged(OldType, NewType);
	if (!AnimationComp) return;
	FString SectionToPlay = TEXT("equip_torch");
	if (NewType == EHoldingObjectType::GunSword) SectionToPlay = TEXT("equip_gun_word");
	AnimationComp->PlayCustomMontage(TEXT("equip_weapon"), FName(SectionToPlay), 1.0f);
}

void ACharacterPlayerHuman::HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails)
{
	Super::HandleDelegate_OnTookDamage_Implementation(DamageDetails);
	if (!StateMachineComp) return;

	bool bShouldEvade = false;
	if (UPlayerBaseState* CurrentState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
	{
		if (CurrentState->GetStateID().Equals(TEXT("player_human_dash_state")))
		{
			bShouldEvade = true;
		}
	}
	if (bShouldEvade) OnEvadeAttackWithDashing(DamageDetails);
	else OnGetHit(DamageDetails);
}

void ACharacterPlayerHuman::OnEvadeAttackWithDashing_Implementation(UDamageDetails* DamageDetails)
{

}

void ACharacterPlayerHuman::OnGetHit_Implementation(UDamageDetails* DamageDetails)
{
	ApplyDamageToHealthStat(DamageDetails);
}
