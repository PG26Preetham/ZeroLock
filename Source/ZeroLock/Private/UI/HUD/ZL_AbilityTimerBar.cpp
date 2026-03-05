// Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_AbilityTimerBar.h"

#include "MVVMSubsystem.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilityTimerProgressBar.h"
#include "View/MVVMView.h"

void UZL_AbilityTimerBar::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZL_VM_AbilityTimerProgressBar* BarData = Cast<UZL_VM_AbilityTimerProgressBar>(ListItemObject);
	if (!BarData) return;
	SetViewModel(BarData);
}

void UZL_AbilityTimerBar::SetViewModel(UZL_VM_AbilityTimerProgressBar* InViewModel)
{
	VM_Progress = InViewModel;
	if (!VM_Progress) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{
		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{
			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_AbilityTimerProgressBar"), VM_Progress))
			{
				
			}
		}
	}
}
