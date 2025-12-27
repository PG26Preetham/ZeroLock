// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_AbilityIcon.h"

#include "MVVMSubsystem.h"
#include "Components/Image.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilityIcon.h"
#include "View/MVVMView.h"


void UZL_HUD_AbilityIcon::SetViewModel(UZL_VM_AbilityIcon* InViewModel)
{
	VM_AbilityIcon = InViewModel;
	if (!VM_AbilityIcon) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_AbilityIcon"), VM_AbilityIcon))
			{
			
				if (AbilityIcon && VM_AbilityIcon)
				{
					AbilityIcon->SetBrushFromTexture(VM_AbilityIcon->GetIconTexture());
				}
			}
		}
	}
	
}
