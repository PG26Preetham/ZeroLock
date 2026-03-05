// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Abilities/ZL_VM_ProgressionStack.h"

#include "UI/MVVM/Abilities/ZL_VM_AbilityTimerProgressBar.h"

void UZL_VM_ProgressionStack::SetProgressionMeters(TMap<FName, UZL_VM_AbilityTimerProgressBar*> Meters)
{
	ProgressionMeters = Meters;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ProgressionMeters);
}

void UZL_VM_ProgressionStack::AddProgressMeter(UZL_VM_AbilityTimerProgressBar* Meter)
{
	if (!ProgressionMeters.Find(FName(Meter->GetAbilityName().ToString())))
	{
		ProgressionMeters.Add(FName(Meter->GetAbilityName().ToString()),Meter);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ProgressionMeters);
	}
}

void UZL_VM_ProgressionStack::RemoveProgressMeter(UZL_VM_AbilityTimerProgressBar* Meter)
{
	ProgressionMeters.Remove(FName(Meter->GetAbilityName().ToString()));
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ProgressionMeters);
}

