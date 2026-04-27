// Copyright Preetham Mukundan (C) 2026


#include "UI/Settings/ZL_SettingsSubMenu.h"

#include "MVVMSubsystem.h"
#include "Components/ScrollBox.h"
#include "UI/Settings/SettingHandlerBase.h"
#include "UI/Settings/MVVM/SettingBaseViewModel.h"
#include "UI/Settings/MVVM/SubMenuManagerViewModel.h"
#include "UI/Settings/MVVM/SliderBaseRow.h"
#include "View/MVVMView.h"

void UZL_SettingsSubMenu::CF_List_To_ScrollBox(TArray<USettingBaseViewModel*> ArrayToWorkWith)
{
	ScrollBox->ClearChildren();
	for (USettingBaseViewModel* ViewModel : ArrayToWorkWith)
	{
		if (ViewModel->GetInteractionType() ==  ESettingInteractionType::Slider)
		{
			USliderBaseRow* SliderWidget = CreateWidget<USliderBaseRow>(this,SliderSubClass);
			SliderWidget->SetViewModel(ViewModel);
			ScrollBox->AddChild(SliderWidget);
		}
	}
}

void UZL_SettingsSubMenu::SetupVM(USubMenuManagerViewModel* VM)
{
	MyVM =  VM;
	if (!MyVM)
	{
		return;
	}
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{
		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{
			if (bool bSuccess = WidgetView->SetViewModel(FName("SubMenuManagerViewModel"), MyVM))
			{
				CF_List_To_ScrollBox(MyVM->SettingsArray);
			}
		}
	}
}
