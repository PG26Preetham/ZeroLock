// Copyright Preetham Mukundan (C) 2026


#include "UI/Settings/ZL_SettingsSubMenu.h"

#include "Components/ScrollBox.h"
#include "UI/Settings/SettingHandlerBase.h"
#include "UI/Settings/MVVM/SettingBaseViewModel.h"
#include "UI/Settings/MVVM/SliderBaseRow.h"

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
