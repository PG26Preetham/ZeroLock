// Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_AbilityToolTips.h"

#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilityIcon.h"
#include "MVVMSubsystem.h"
#include "View/MVVMView.h"

void UZL_AbilityToolTips::SetViewModel(UZL_VM_AbilityIcon* InViewModel)
{
	
	VM_AbilityIcon = InViewModel;
	if (!VM_AbilityIcon) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_AbilityIcon"),VM_AbilityIcon))
			{
				AbilityDescription->SetText(VM_AbilityIcon->AbilityDescription);
				VM_AbilityLevelUp(VM_AbilityIcon->AbilityLevel);
				Abilityname->SetText(VM_AbilityIcon->AbilityName);
			}
		}
	}
}

void UZL_AbilityToolTips::VM_AbilityLevelUp(int32 newLevel)
{
	if (!DisabledStyle || !EnabledStyle) return;
	B_Level1->SetStyle(DisabledStyle);
	B_Level2->SetStyle(DisabledStyle);
	B_Level3->SetStyle(DisabledStyle);

	if (newLevel >=1)
	{
		B_Level1->SetStyle(EnabledStyle);
	}
	if (newLevel >=2)
	{
		B_Level2->SetStyle(EnabledStyle);
	}
	if (newLevel >=3)
	{
		B_Level3->SetStyle(EnabledStyle);
	}
}
