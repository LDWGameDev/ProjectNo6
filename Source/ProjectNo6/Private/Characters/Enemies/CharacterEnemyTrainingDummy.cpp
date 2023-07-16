// Copy Right LDW. All Rights Reserved.


#include "Characters/Enemies/CharacterEnemyTrainingDummy.h"
#include "Components/AnimationComponent.h"
#include "System/Combat/DamageDetails.h"
#include "System/Combat/CombatMove.h"

ACharacterEnemyTrainingDummy::ACharacterEnemyTrainingDummy()
{
	
}

void ACharacterEnemyTrainingDummy::HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails)
{
	Super::HandleDelegate_OnTookDamage_Implementation(DamageDetails);
	if (!DamageDetails || !DamageDetails->IsDamageDetailsValid() || bIsDeath) return;

	float StepBackDistance = 80.0f;
	if(DamageDetails->InstigatorCombatMove->HitType == EHitType::LightHit) StepBackDistance = 40.0f;
	StepBackWhenGetHit(DamageDetails, StepBackDistance);

	RotateToFaceInstigatorWhenGetHit(DamageDetails);
	PlayGetHitOutlineEffect(4.0f);
	if (AnimationComp)
	{
		AnimationComp->PlayCustomMontage(TEXT("actions"), TEXT("get_hit"));
	}
}

