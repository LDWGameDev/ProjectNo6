// Copy Right LDW. All Rights Reserved.


#include "System/Stat/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bHasPowerStat)
	{
		PowerStat = NewObject<UStatBase>(this);
		PowerStat->BaseValue = 5;
		PowerStat->MinValue = 0;
		PowerStat->MaxValue = 5;
		PowerStat->Type = ECustomStatType::Power;
		StatArray.Add(PowerStat);
	}
	for (UStatBase* TempStat : StatArray)
	{
		if (!TempStat) continue;
		TempStat->OnStatValueChangedDelegate.AddDynamic(this, &UStatComponent::HandleOnStatValueChanged);
		TempStat->Initialize(this);
	}
}

UStatBase* UStatComponent::GetStat(ECustomStatType StatToGet)
{
	for (UStatBase* TempStat : StatArray)
	{
		if (TempStat && TempStat->Type == StatToGet)
		{
			return TempStat;
		}
	}
	return nullptr;
}

void UStatComponent::StartDelayRegeneratePower()
{
	if (PowerStat && PowerStat->GetValue() < PowerStat->MaxValue && ActionDelayRegenerate > 0.0f)
	{
		FTimerDelegate WaitRegenerateDelegate;
		WaitRegenerateDelegate.BindLambda([&]() {
			if (GetWorld() && RegenerateInterval > 0.0f)
			{
				GetWorld()->GetTimerManager().SetTimer(HandleRegenerate, this, &UStatComponent::RegeneratePower, RegenerateInterval, true);
			}
			});
		GetWorld()->GetTimerManager().SetTimer(HandleWaitRegenerate, WaitRegenerateDelegate, ActionDelayRegenerate, false);
	}
}

void UStatComponent::StopRegeneratePower()
{
	if (GetWorld())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(HandleWaitRegenerate)) GetWorld()->GetTimerManager().ClearTimer(HandleWaitRegenerate);
		if (GetWorld()->GetTimerManager().IsTimerActive(HandleRegenerate)) GetWorld()->GetTimerManager().ClearTimer(HandleRegenerate);
	}
}

void UStatComponent::HandleOnStatValueChanged(UStatBase* ChangedStat, int32 OldValue, int32 NewValue)
{
	if (OnStatValueChangedDelegate.IsBound())
	{
		OnStatValueChangedDelegate.Broadcast(ChangedStat, OldValue, NewValue);
	}
	
	// if (PowerStat && NewValue < OldValue && GetWorld() && GetWorld()->)
}

void UStatComponent::HandleOnStatMinMaxValueChanged(UStatBase* ChangedStat, int32 OldMin, int32 OldMax, int32 NewMin, int32 NewMax)
{
	if (OnStatMinMaxValueChangedDelegate.IsBound())
	{
		OnStatMinMaxValueChangedDelegate.Broadcast(ChangedStat, OldMin, OldMax, NewMin, NewMax);
	}
}

void UStatComponent::RegeneratePower()
{
	if (PowerStat && PowerStat->GetValue() < PowerStat->MaxValue)
	{
		PowerStat->AddValue(1);
	}
	else if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HandleRegenerate);
	}
}
