// Copy Right LDW. All Rights Reserved.

#include "System/Stat/StatBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/Stat/StatComponent.h"

UStatBase::UStatBase()
{

}

void UStatBase::Initialize_Implementation(UStatComponent* OwnerStatComponent)
{
	SetCurrentValue(BaseValue, true);
}

int32 UStatBase::GetValue()
{
	return CurrentValue;
}

bool UStatBase::AddValue(int32 AmountToAdd)
{
	int32 OldValue = CurrentValue;
	int32 NewValue = FMath::Clamp(CurrentValue + AmountToAdd, MinValue, MaxValue);

	if (NewValue == OldValue) return false;
	CurrentValue = NewValue;

	if (OnStatValueChangedDelegate.IsBound())
	{
		OnStatValueChangedDelegate.Broadcast(this, OldValue, CurrentValue);
	}
	return true;
}

bool UStatBase::SetCurrentValue(int32 NewValue, bool bClampToMinMax /*= true*/)
{
	if (bClampToMinMax || (!bClampToMinMax && (UKismetMathLibrary::InRange_IntInt(NewValue, MinValue, MaxValue, true, true))))
	{
		int32 OldValue = CurrentValue;
		CurrentValue = FMath::Clamp(NewValue, MinValue, MaxValue);
		if (CurrentValue != OldValue && OnStatValueChangedDelegate.IsBound())
		{
			OnStatValueChangedDelegate.Broadcast(this, OldValue, CurrentValue);
		}
		return true;
	}
	return false;
}

bool UStatBase::SetMinMaxValue(int32 NewMinValue, int32 NewMaxValue)
{
	if ((NewMaxValue <= NewMinValue) || (NewMinValue == MinValue && NewMaxValue == MaxValue)) return false;
	int32 OldMin = MinValue;
	int32 OldMax = MaxValue;

	MinValue = NewMinValue;
	MaxValue = NewMaxValue;

	int32 NewCurrentValue = FMath::Clamp(CurrentValue, MinValue, MaxValue);
	SetCurrentValue(NewCurrentValue);

	if (OnStatMinMaxChangedDelegate.IsBound())
	{
		OnStatMinMaxChangedDelegate.Broadcast(this, OldMin, OldMax, MinValue, MaxValue);
	}
	return true;
}
