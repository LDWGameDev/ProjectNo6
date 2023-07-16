// Copy Right LDW. All Rights Reserved.


#include "System/StateMachine/Player/PlayerBaseState.h"
#include "System/StateMachine/Player/PlayerStateMachineComponent.h"
#include "CustomInterface.h"
#include "System/Stat/StatComponent.h"
#include "System/Stat/StatBase.h"
#include "Characters/Player/CharacterPlayerBase.h"


void UPlayerBaseState::EnterState_Implementation()
{
	Super::EnterState_Implementation();
	bIsStateValid = CheckValidState();
	if (!OwnerCharacterPlayerBase) return;

	if (OnStateEnteredDelegate.IsBound())
	{
		OnStateEnteredDelegate.Broadcast(this);
	}
	if (OnStateStartedCooldownDelegate.IsBound())
	{
		OnStateStartedCooldownDelegate.Broadcast(this);
	}

	bCachedDashInput = false;
	bCachedHAInput = false;
	bCachedLAInput = false;

	if (bEnterStopRegeneratePower && OwnerCharacterPlayerBase->GetStatComp())
	{
		OwnerCharacterPlayerBase->GetStatComp()->StopRegeneratePower();
	}
}

void UPlayerBaseState::TickState_Implementation(float DeltaTime)
{
	Super::TickState_Implementation(DeltaTime);
}

void UPlayerBaseState::ExitState_Implementation()
{	
	Super::ExitState_Implementation();

	if (bExitStartDelayRegeneratePower && OwnerCharacterPlayerBase && OwnerCharacterPlayerBase->GetStatComp())
	{
		OwnerCharacterPlayerBase->GetStatComp()->StartDelayRegeneratePower();
	}
}

void UPlayerBaseState::InitState_Implementation(ULDWStateMachineComponent* StateMachineComp, AActor* OwnerActor)
{
	Super::InitState_Implementation(StateMachineComp, OwnerActor);
	OwnerCharacterPlayerBase = Cast<ACharacterPlayerBase>(OwnerActor);
}

void UPlayerBaseState::HandleActionString_Implementation(const FString& ActionID)
{
	Super::HandleActionString_Implementation(ActionID);
	if (!bIsInState) return;
}

void UPlayerBaseState::ChangeState_Implementation(const FString& NewStateID)
{
	if (!OwnerStateMachineComp || !OwnerCharacterPlayerBase) return;
	if (UPlayerBaseState* NewState = OwnerStateMachineComp->FindState<UPlayerBaseState>(NewStateID))
	{
		bool bShouldChangeState = true;
		if (NewState->CheckPowerRequired())
		{
			NewState->ApplyPowerRequired();
			OwnerStateMachineComp->ChangeState(NewStateID);
		}
	}
}

void UPlayerBaseState::OnInputActionTriggered_Movement_Implementation(const FVector2D& InputValue)
{
	
}

void UPlayerBaseState::OnInputActionTriggered_Dash_Implementation(bool InputValue)
{
	bCachedDashInput = true;
}

void UPlayerBaseState::OnInputAction_LightAttack_Started_Implementation()
{
	bCachedLAInput = true;
}

void UPlayerBaseState::OnInputAction_LightAttack_Triggered_Implementation()
{
	
}

void UPlayerBaseState::OnInputAction_LightAttack_Completed_Implementation()
{
	
}

void UPlayerBaseState::OnInputAction_LightAttack_Canceled_Implementation()
{
	
}

void UPlayerBaseState::OnInputAction_HeavyAttack_Started_Implementation()
{
	bCachedHAInput = true;
}

void UPlayerBaseState::OnInputAction_HeavyAttack_Triggered_Implementation()
{

}

void UPlayerBaseState::OnInputAction_HeavyAttack_Completed_Implementation()
{

}

void UPlayerBaseState::OnInputAction_HeavyAttack_Canceled_Implementation()
{

}

bool UPlayerBaseState::CheckPowerRequired()
{
	if (PowerRequired <= 0 || !OwnerCharacterPlayerBase || !OwnerCharacterPlayerBase->GetStatComp()) return true;
	if (UStatBase* PowerStat = OwnerCharacterPlayerBase->GetStatComp()->GetStat(ECustomStatType::Power))
	{
		return (PowerStat->GetValue() >= PowerRequired);
	}
	return false;
}

void UPlayerBaseState::ApplyPowerRequired()
{
	if (PowerRequired <= 0 || !OwnerCharacterPlayerBase || !OwnerCharacterPlayerBase->GetStatComp()) return;
	if (UStatBase* PowerStat = OwnerCharacterPlayerBase->GetStatComp()->GetStat(ECustomStatType::Power))
	{
		PowerStat->AddValue(-PowerRequired);
	}
}

bool UPlayerBaseState::CheckValidState_Implementation()
{
	return (OwnerStateMachineComp && OwnerCharacterPlayerBase && bIsInState);
}

void UPlayerBaseState::ClearCachedInputs()
{
	bCachedDashInput = false;
	bCachedLAInput = false;
	bCachedHAInput = false;
}
