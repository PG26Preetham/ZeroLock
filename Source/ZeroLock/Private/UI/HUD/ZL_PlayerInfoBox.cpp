//Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_PlayerInfoBox.h"

#include <string>

#include "CommonTextBlock.h"
#include "MVVMSubsystem.h"
#include "Zero_BasePlayerState.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "UI/MVVM/GameStats/ZL_VM_PlayerInfo.h"
#include "UI/MVVM/ZL_VM_Attributes.h"
#include "View/MVVMView.h"


void UZL_PlayerInfoBox::SetViewModel(UZL_VM_PlayerInfo* InViewModel)
{
	myVM = InViewModel;
	if (!myVM) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_PlayerInfo"), myVM))
			{
				PlayerIcon->SetBrushFromTexture(myVM->GetIcon());
				PlayerName->SetText(myVM->GetPlayerName());
				Kill->SetText(FText::FromString(FString::FromInt(myVM->GetKills())));
				Death->SetText(FText::FromString(FString::FromInt(myVM->GetDeaths())));
				Assist->SetText(FText::FromString(FString::FromInt(myVM->GetAssists())));
				SetAttributeVM(myVM->GetVM_Attributes());
			}
		}
	}
}

void UZL_PlayerInfoBox::SetAttributeVM(UZL_VM_Attributes* AtVM)
{
	myAttributes = AtVM;
	if (!myAttributes) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_Attributes"), myAttributes))
			{
				HealthBar->SetPercent(myAttributes->GetHealthPercentage());
			}
		}
	}
}
