//Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_HUD_AbilityIcon.h"

#include <string>

#include "CommonTextBlock.h"
#include "MVVMSubsystem.h"
#include "Components/Image.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilityIcon.h"
#include "View/MVVMView.h"
#include "UI/HUD/ZL_AbilityToolTips.h"
#include "ZeroLock/ZeroLock.h"


void UZL_HUD_AbilityIcon::SetViewModel(UZL_VM_AbilityIcon* InViewModel)
{
	AbilityChargesText->SetVisibility(ESlateVisibility::Hidden);
	Ability_StackText->SetVisibility(ESlateVisibility::Hidden);
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
					if (VM_AbilityIcon->GetbHasStacks())
					{
						Ability_StackText->SetVisibility(ESlateVisibility::Visible);
						FString StackCountText = FString::FromInt(VM_AbilityIcon->GetStackNum());
						Ability_StackText->SetText(FText::FromString(StackCountText));
					}
					if (VM_AbilityIcon->GetbHasCharges())
					{
						AbilityChargesText->SetVisibility(ESlateVisibility::Visible);
						FString ChargeCountText = FString::FromInt(VM_AbilityIcon->GetAbilityCharges());
						AbilityChargesText->SetText(FText::FromString(ChargeCountText));
					}
				}
				if (TooltipWidgetClass)
				{

					UZL_AbilityToolTips* TooltipWidget = CreateWidget<UZL_AbilityToolTips>(GetOwningPlayer(),TooltipWidgetClass);

					if (TooltipWidget)
					{
						ZLOG("ToolTipsSetup")
						TooltipWidget->SetViewModel(InViewModel);
						SetToolTip(TooltipWidget);
					}
				}
	
			}
		}
	}
	
}

void UZL_HUD_AbilityIcon::NativeOnClicked()
{
	Super::NativeOnClicked();
	
}

FReply UZL_HUD_AbilityIcon::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (VM_AbilityIcon)
	{
		VM_AbilityIcon->IncrementAbilityLevel();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}
