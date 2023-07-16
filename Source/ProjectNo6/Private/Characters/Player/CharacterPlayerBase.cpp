// Copy Right LDW. All Rights Reserved.

#include "Characters/Player/CharacterPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "System/StateMachine/Player/PlayerStateMachineComponent.h"
#include "System/StateMachine/Player/PlayerBaseState.h"
#include "Components/AnimationComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TransformComponent.h"
#include "Characters/Player/PlayerControllerGameplay.h"
#include "Components/FXComponent.h"
#include "System/Combat/CombatComponent.h"
#include "Components/GameplayTagComponent.h"
#include "System/Combat/CombatMove.h"
#include "System/Stat/StatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/Equipment/EquipmentComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CustomFunctionLibrary.h"
#include "System/Combat/DamageDetails.h"
#include "GameInstanceCustom.h"
#include "Characters/Enemies/CharacterEnemyBase.h"

ACharacterPlayerBase::ACharacterPlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MainSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp0"));
	MainSpringArmComp->SetupAttachment(RootComponent);
	MainCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp0"));
	MainCameraComp->SetupAttachment(MainSpringArmComp);
	MainCameraComp->FieldOfView = 70.0f;

	UnderFootDirectionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("UnderFootDirectionWidgetComp"));
	UnderFootDirectionWidgetComp->SetupAttachment(RootComponent);

	PersistentStaminaWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PersistentStaminaWidgetComp"));
	PersistentStaminaWidgetComp->SetupAttachment(RootComponent);
	
	TorchPointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchPointLightComp"));
	TorchPointLightComp->SetupAttachment(GetRootComponent());

	StateMachineComp = CreateDefaultSubobject<UPlayerStateMachineComponent>(TEXT("StateMachineComp"));
	AnimationComp = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComp"));
	TransformComp = CreateDefaultSubobject<UTransformComponent>(TEXT("TransformComp"));
	FXComp = CreateDefaultSubobject<UFXComponent>(TEXT("FXComp"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	TagComp = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	EquipComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));

	CombatComp->OnCombatMoveHitTargetsDelegate.AddDynamic(this, &ACharacterPlayerBase::HandleDelegate_OnCombatMoveHitTargets);
	CombatComp->OnTookDamageDelegate.AddDynamic(this, &ACharacterPlayerBase::HandleDelegate_OnTookDamage);
	EquipComp->OnEquipmentIndexChangedDelegate.AddDynamic(this, &ACharacterPlayerBase::HandleDelegate_OnEquipmentIndexChanged);
	StatComp->OnStatMinMaxValueChangedDelegate.AddDynamic(this, &ACharacterPlayerBase::HandleDelegate_OnStatMinMaxValueChanged);
	StatComp->OnStatValueChangedDelegate.AddDynamic(this, &ACharacterPlayerBase::HandleDelegate_OnStatValueChanged);

	if (MainSpringArmComp)
	{
		MainSpringArmComp->SetRelativeRotation(FRotator(-55.0f, 0.0f, 0.0f));
		MainSpringArmComp->TargetOffset = FVector(-30.0f, 0.0f, 0.0f);
		MainSpringArmComp->TargetArmLength = 1800.0f;

		MainSpringArmComp->bUsePawnControlRotation = false;
		MainSpringArmComp->bInheritPitch = false;
		MainSpringArmComp->bInheritYaw = false;
		MainSpringArmComp->bInheritRoll = false;
	}

	if (GetMesh())
	{
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->bOrientRotationToMovement = false;
		MovementComp->RotationRate = FRotator(0.0f, 750.0f, 0.0f);
		MovementComp->MaxAcceleration = 8000.0f;
	}

	FlowIndex = 0;
}

void ACharacterPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DelayBeginPlayTimerHandle;
	GetWorldTimerManager().SetTimer(DelayBeginPlayTimerHandle, this, &ACharacterPlayerBase::DelayBeginPlay, 0.5f, false);
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
	MainSpringArmComp->TargetArmLength = DefaultCameraLength;
}

void ACharacterPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldRotateToMouse && OwnerGameplayPlayerController)
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OwnerGameplayPlayerController->GetGameplayMouseLocation());
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotateTowardMouseSpeed));
	}
}

void ACharacterPlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (APlayerControllerGameplay* GameplayPlayerController = Cast<APlayerControllerGameplay>(NewController))
	{
		OwnerGameplayPlayerController = GameplayPlayerController;
	}
}

void ACharacterPlayerBase::DelayBeginPlay()
{
	if (StatComp->PowerStat)
	{
		UpdateStamineGemsAmount(StatComp->PowerStat->MaxValue);
	}
}

FVector ACharacterPlayerBase::GetFootLocation()
{
	if (UCapsuleComponent* RootCapsuleComp = GetCapsuleComponent())
	{
		return (GetActorLocation() + FVector::DownVector * (RootCapsuleComp->GetScaledCapsuleHalfHeight()));
	}
	return GetActorLocation();
}

FVector ACharacterPlayerBase::GetGameplayMouseLocation()
{
	if (APlayerControllerGameplay* GameplayController = GetController<APlayerControllerGameplay>())
	{
		return GameplayController->GetGameplayMouseLocation();
	}
	return FVector();
}

void ACharacterPlayerBase::UnderFootDirectionEffect_Implementation(float /*Duration = 0.4f*/)
{
	
}

void ACharacterPlayerBase::CustomPlaySound(FString Id, FVector Location)
{
	UCustomFunctionLibrary::CustomSpawnSoundAtLocation(this, Id, Location);
}

void ACharacterPlayerBase::UpdateStamineGemsAmount_Implementation(int32 Amount)
{
	
}

void ACharacterPlayerBase::UpdateStaminaGemState_Implementation(bool bIncreased)
{

}

void ACharacterPlayerBase::AddInteractableActor(AActor* Actor)
{
	if (OwnerGameplayPlayerController)
	{
		OwnerGameplayPlayerController->AddInteractableActor(Actor);
	}
}

void ACharacterPlayerBase::RemoveInteractableActor(AActor* Actor)
{
	if (OwnerGameplayPlayerController)
	{
		OwnerGameplayPlayerController->RemoveInteractableActor(Actor);
	}
}

void ACharacterPlayerBase::OnInputActionTriggered_Movement(const FVector2D& InputValue)
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputActionTriggered_Movement(InputValue);
		}
	}
	LastMovementInput = InputValue;
}

void ACharacterPlayerBase::OnInputActionCompleted_Movement(const FVector2D& InputValue)
{
	LastMovementInput = FVector2D();
}

void ACharacterPlayerBase::OnInputActionTriggered_Dash(bool InputValue)
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputActionTriggered_Dash(InputValue);
		}
	}
}

void ACharacterPlayerBase::OnInputAction_LightAttack_Started()
{
	bIsHoldingLightAttackInput = true;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_LightAttack_Started();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_LightAttack_Triggered()
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_LightAttack_Triggered();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_LightAttack_Completed()
{
	bIsHoldingLightAttackInput = false;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_LightAttack_Completed();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_LightAttack_Canceled()
{
	bIsHoldingLightAttackInput = false;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_LightAttack_Canceled();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_HeavyAttack_Started()
{
	bIsHoldingHeavyAttackInput = true;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_HeavyAttack_Started();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_HeavyAttack_Triggered()
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_HeavyAttack_Triggered();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_HeavyAttack_Completed()
{
	bIsHoldingHeavyAttackInput = false;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_HeavyAttack_Completed();
		}
	}
}

void ACharacterPlayerBase::OnInputAction_HeavyAttack_Canceled()
{
	bIsHoldingHeavyAttackInput = false;
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			CurrentPlayerBaseState->OnInputAction_HeavyAttack_Canceled();
		}
	}
}

void ACharacterPlayerBase::OnInputActionTriggered_HeavyAttack(bool InputValue)
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			
		}
	}
}

void ACharacterPlayerBase::OnInputActionTriggered_QSkill(bool InputValue)
{
	if (StateMachineComp)
	{
		if (UPlayerBaseState* CurrentPlayerBaseState = StateMachineComp->GetCurrentState<UPlayerBaseState>())
		{
			
		}
	}
}

void ACharacterPlayerBase::OnInputActionTriggered_MiddleMouseScroll(float InputValue)
{
	if (EquipComp && InputValue != 0.0f)
	{
		EquipComp->UpdateCurrentHoldingType(InputValue > 0);
	}
}

void ACharacterPlayerBase::MoveWithInput(const FVector2D& MovementInput)
{
	if (!MovementInput.IsNearlyZero())
	{
		// TODO: change input value according to movement stat value of character, such as base speed, buffs
		AddMovementInput(FVector::ForwardVector * MovementInput.Y, 1.0f);
		AddMovementInput(FVector::RightVector * MovementInput.X, 1.0f);
	}
}

void ACharacterPlayerBase::HandleDelegate_OnCombatMoveHitTargets_Implementation(UCombatMove* CombatMove, const TArray<AActor*>& HitTargets)
{
	if (!CombatMove || HitTargets.Num() == 0) return;
	if (!FXComp || !StatComp) return;

	bool ShouldSpawnSound = false;
	for (AActor* TempActor : HitTargets)
	{
		if (ACharacterEnemyBase* TargetAsEnemy = Cast<ACharacterEnemyBase>(TempActor)) 
		{
			if (TargetAsEnemy->bIsDeath) continue;
		}
		ShouldSpawnSound = true;
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

	if (ShouldSpawnSound)
	{
		if (UGameInstanceCustom* CustomGI = GetWorld()->GetGameInstance<UGameInstanceCustom>())
		{
			CustomGI->CustomSpawnSoundAtLocation(CombatMove->HitTargetSF, GetActorLocation());
		}
	}
}

void ACharacterPlayerBase::ForceDie()
{
	if (StatComp)
	{
		if (UStatBase* HealthStat = StatComp->GetStat(ECustomStatType::HP))
		{
			HealthStat->AddValue(-1000);
		}
	}
	OnDied();
}

void ACharacterPlayerBase::HandleDelegate_OnEquipmentIndexChanged(EHoldingObjectType OldType, EHoldingObjectType NewType)
{
	FlowIndex = (NewType == EHoldingObjectType::GunSword) ? 1 : 0;
}

void ACharacterPlayerBase::HandleDelegate_OnStatMinMaxValueChanged(UStatBase* ChangedStat, int32 OldMin, int32 OldMax, int32 NewMin, int32 NewMax)
{
	if (!ChangedStat) return;
	if (ChangedStat->Type == ECustomStatType::Power)
	{
		UpdateStamineGemsAmount(NewMax);
	}
}

void ACharacterPlayerBase::HandleDelegate_OnStatValueChanged(UStatBase* ChangedStat, int32 OldValue, int32 NewValue)
{
	if (!ChangedStat) return;
	UpdateStaminaGemState(NewValue > OldValue);
}

void ACharacterPlayerBase::HandleDelegate_OnTookDamage_Implementation(UDamageDetails* DamageDetails)
{
	
}

void ACharacterPlayerBase::OnDied_Implementation()
{
	bIsDeath = true;
}

void ACharacterPlayerBase::ApplyDamageToHealthStat(UDamageDetails* DamageDetails)
{
	if (!DamageDetails || bIsDeath) return;
	if (StatComp)
	{
		if (UStatBase* HealthStat = StatComp->GetStat(ECustomStatType::HP))
		{
			HealthStat->AddValue(-DamageDetails->Damage);
			if (HealthStat->GetValue() <= 0)
			{
				OnDied();
			}
		}
	}
}