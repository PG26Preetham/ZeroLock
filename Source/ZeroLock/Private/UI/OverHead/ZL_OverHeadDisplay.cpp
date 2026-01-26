// Copyright Preetham Mukundan (C) 2026


#include "UI/OverHead/ZL_OverHeadDisplay.h"
#include "UI/MVVM/ZL_VM_Attributes.h"
#include "MVVMSubsystem.h"
#include "Components/ProgressBar.h"
#include "View/MVVMView.h"
#include "ZeroLock/ZeroLock.h"

void UZL_OverHeadDisplay::SetViewModel(UZL_VM_Attributes* InViewModel)
{
	
	VM_Attributes = InViewModel;
	if (!VM_Attributes) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{
		
		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					
			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_Attributes"), VM_Attributes))
			{
				ZLOG_COLOR_TIME("Init In Widget",FColor::Blue,10);
				HealthBar->SetPercent(VM_Attributes->GetHealthPercentage());
	
			}
		}
	}
}
