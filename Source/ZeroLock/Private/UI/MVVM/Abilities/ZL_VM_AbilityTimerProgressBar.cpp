// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Abilities/ZL_VM_AbilityTimerProgressBar.h"

void UZL_VM_AbilityTimerProgressBar::SetProgressionLevel(float level)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ProgressionLevel,level))
	{
		
	}
}

void UZL_VM_AbilityTimerProgressBar::SetAbilityName(FText name)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(AbilityName,name))
	{
		
	}
}
