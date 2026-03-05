// Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_AbilityProgressBar_Stack.h"
#include "CommonListView.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilityTimerProgressBar.h"

void UZL_AbilityProgressBar_Stack::UpdateTheList(TMap<FName, UZL_VM_AbilityTimerProgressBar*> NewList)
{
	if (ProgressBarList)
	{
		ProgressBarList->ClearListItems();
		for (const TPair<FName, UZL_VM_AbilityTimerProgressBar*>& Pair : NewList)
		{
		
			if (Pair.Value)
			{
				ProgressBarList->AddItem(Pair.Value);
			}
		}
	}
}
