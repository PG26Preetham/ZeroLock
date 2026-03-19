// Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_AbilityChargePhase.h"
#include "UI/MVVM/Abilities/ZL_VM_ChargePercent.h"
#include "MVVMSubsystem.h"
#include "View/MVVMView.h"

void UZL_AbilityChargePhase::SetViewModel(UZL_VM_ChargePercent* ViewModel)
{
	MyVM = ViewModel;
	if (!MyVM) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_ChargePercent"),MyVM))
			{
				
			}
		}
	}
}
