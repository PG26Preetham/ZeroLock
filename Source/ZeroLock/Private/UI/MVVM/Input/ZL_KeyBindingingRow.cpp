// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Input/ZL_KeyBindingingRow.h"

#include "MVVMSubsystem.h"
#include "View/MVVMView.h"
#include "ZeroLock/ZeroLock.h"
#include"ZeroLock/Public/UI/MVVM/Input/ZL_VM_KeyBindingRow.h"

void UZL_KeyBindingingRow::SetRowViewModel(UZL_VM_KeyBindingRow* InViewModel)
{
	VM_Attributes = InViewModel;
	if (!VM_Attributes) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{
		
		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					
			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_KeyBindingRow"), VM_Attributes))
			{
				ZLOG_COLOR_TIME("Init In Widget",FColor::Blue,10);	
			}
		}
	}
}
