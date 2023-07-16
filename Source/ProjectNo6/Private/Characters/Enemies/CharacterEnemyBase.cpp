// Copy Right LDW. All Rights Reserved.


#include "Characters/Enemies/CharacterEnemyBase.h"
#include "Components/AnimationComponent.h"
#include "Components/TransformComponent.h"
#include "Components/GameplayTagComponent.h"
#include "Components/FXComponent.h"
#include "System/Combat/CombatComponent.h"
#include "System/StateMachine/Enemy/EnemyStateMachineComponent.h"
#include "System/Combat/CombatMove.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "System/Combat/DamageDetails.h"
#include "System/Stat/StatBase.h"
#include "System/Stat/StatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIModule/Classes/AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "CustomData.h"
#include "GameInstanceCustom.h"


ACharacterEnemyBase::ACharacterEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AnimationComp = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComp"));
	TransformComp = CreateDefaultSubobject<UTransformComponent>(TEXT("TransformComp"));
	FXComp = CreateDefaultSubobject<UFXComponent>(TEXT("FXComp"));
	StateMachineComp = CreateDefaultSubobject<UEnemyStateMachineComponent>(TEXT("EnemyStateMachineComp"));
	TagComp = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComp"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));

	CombatComp->OnTookDamageDelegate.AddDynamic(this, &ACharacterEnemyBase::HandleDelegate_OnTookDamage);
	CombatComp->OnTriggeredCombatMoveDelegate.AddDynamic(this, &ACharacterEnemyBase::HandleDelegate_OnTriggeredCombatMove);
	CombatComp->OnCombatMoveHitTargetsDelegate.AddDynamic(this, &ACharacterEnemyBase::HandleDelegate_OnCombatMoveHitTarget);
}

void ACharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Create dynamic material instances
	if (USkeletalMeshComponent* OwnerMesh = GetMesh())
	{
		TArray<UMaterialInterface*> MeshMaterialArray = OwnerMesh->GetMaterials();
		for (int i = 0; i < MeshMaterialArray.Num(); i++)
		{
			if (MeshMaterialArray[i])
			{
				UMaterialInstanceDynamic* DynamicMatInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MeshMaterialArray[i]);
				if (DynamicMatInstance)
				{
					OwnerMesh->SetMaterial(i, DynamicMatInstance);
				}
			}
		}
	}
}

void ACharacterEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterEnemyBase::HandleDelegate_OnTriggeredCombatMove(UCombatMove* CombatMove)
{
	
}

void ACharacterEnemyBase::HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails)
{
	if (bIsDeath) return;
	ReduceHealthWhenGetHit(DamageDetails);
}

void ACharacterEnemyBase::StepBackWhenGetHit(UDamageDetails* DamageDetails, float StepBackDistance /*= 150.0f*/)
{
	if (!TransformComp || !DamageDetails || !DamageDetails->IsDamageDetailsValid()) return;
	FVector InstigatorForwardDirection = DamageDetails->InstigatorActor->GetActorForwardVector();
	InstigatorForwardDirection.Z = 0.0f;
	InstigatorForwardDirection.Normalize();
	FVector TargetLocation = GetActorLocation() + (InstigatorForwardDirection * StepBackDistance);
	TransformComp->MoveToLocation(TargetLocation, 0.1f, EAlphaCurveType::Linear, true);
}

void ACharacterEnemyBase::RotateToFaceInstigatorWhenGetHit(UDamageDetails* DamageDetails)
{
	if(!DamageDetails || !DamageDetails->IsDamageDetailsValid()) return;
	FRotator LookAtInstigatorRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageDetails->InstigatorActor->GetActorLocation());
	LookAtInstigatorRotation.Pitch = 0.0f;
	LookAtInstigatorRotation.Roll = 0.0f;
	SetActorRotation(LookAtInstigatorRotation);
}

void ACharacterEnemyBase::ReduceHealthWhenGetHit(UDamageDetails* DamageDetails)
{
	if (!DamageDetails || !StatComp) return;
	if (UStatBase* HealthStat = StatComp->GetStat(ECustomStatType::HP))
	{
		HealthStat->AddValue(-DamageDetails->Damage);
		if (HealthStat->GetValue() <= 0)
		{
			OnDied();
		}
	}
}

void ACharacterEnemyBase::PlayGetHitOutlineEffect(float OutlinePower /*= 6.0f*/)
{
	SetScalarParameterOnMaterials(UCustomData::AuraOutline, 1.0f);
	SetScalarParameterOnMaterials(UCustomData::AuraOutlinePower, OutlinePower);
	FTimerDelegate DisableAuraOutllineTimerHandle = FTimerDelegate::CreateUObject(this, &ACharacterEnemyBase::SetScalarParameterOnMaterials, UCustomData::AuraOutline, 0.0f);
	GetWorld()->GetTimerManager().SetTimer(MaterialAuraOutlineTimerHandle, DisableAuraOutllineTimerHandle, 0.1f, false);
}

void ACharacterEnemyBase::OnDied_Implementation()
{
	OnDiedCode();
}

void ACharacterEnemyBase::OnDiedCode()
{
	bIsDeath = true;
	if (OnDiedDelegate.IsBound())
	{
		OnDiedDelegate.Broadcast(this);
	}

	if (UGameInstanceCustom* CustomGameInstance = Cast<UGameInstanceCustom>(GetWorld()->GetGameInstance()))
	{
		CustomGameInstance->OnEnemyDied(this);
	}
}

void ACharacterEnemyBase::HandleDelegate_OnCombatMoveHitTarget(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets)
{
	if (!CombatMove || HitTargets.Num() == 0 || !FXComp) return;

	for (AActor* TempActor : HitTargets)
	{
		if (CombatMove->HitTargetFXArray.Num() > 0 && TempActor)
		{
			for (FHitTargetFX& HitFX : CombatMove->HitTargetFXArray)
			{
				FVector LookAtTargetDirection = TempActor->GetActorLocation() - GetActorLocation();
				LookAtTargetDirection.Z = 0.0f;
				LookAtTargetDirection.Normalize();

				FVector FXLocation = LookAtTargetDirection * HitFX.Location.X + GetActorLocation();
				FXLocation.Z = FXLocation.Z + HitFX.Location.Z;
				FXComp->SpawnFXAtLocation(HitFX.FXId, FXLocation, HitFX.Rotation, HitFX.Scale, false);
			}
		}
	}
}

void ACharacterEnemyBase::ForceOnDied()
{
	OnDied();
}

void ACharacterEnemyBase::SetScalarParameterOnMaterials(FName ParaName, float ScalarValue)
{
	if (GetMesh())
	{
		GetMesh()->SetScalarParameterValueOnMaterials(ParaName, ScalarValue);
	}
}

void ACharacterEnemyBase::SetVectorParameterOnMaterials(FName ParaName, FVector VectorValue)
{
	if (GetMesh())
	{
		GetMesh()->SetVectorParameterValueOnMaterials(ParaName, VectorValue);
	}
}

const UBTNode* ACharacterEnemyBase::GetCurrentActiveBTNode()
{
	if (GetOwner())
	{
		if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(GetOwner()->GetComponentByClass(UBehaviorTreeComponent::StaticClass())))
		{
			return BehaviorTreeComp->GetActiveNode();
		}
	}
	return nullptr;
}
