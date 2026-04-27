// Copyright Preetham Mukundan (C) 2026


#include "UI/Settings/MVVM/SettingsManagerViewModel.h"

#include "UI/Settings/SettingHandlerBase.h"
#include "UI/Settings/MVVM/BooleanSettingViewModel.h"
#include "UI/Settings/MVVM/ScalarSettingViewModel.h"

void USettingsManagerViewModel::BuildSettingsFromDataTable(UDataTable* MasterTable)
{
	if (!MasterTable) return;

	static const FString ContextString(TEXT("SettingsManagerContext"));
	TArray<FMasterSettingRow*> Rows;
	MasterTable->GetAllRows<FMasterSettingRow>(ContextString, Rows);

	for (FMasterSettingRow* Row : Rows)
	{
		if (!Row || !Row->CustomHandlerClass) continue;

		USettingBaseViewModel* NewVM = nullptr;
		switch (Row->InteractionType)
		{
		case ESettingInteractionType::Slider:
			NewVM = NewObject<UScalarSettingViewModel>(this);
			break;
		case ESettingInteractionType::Toggle:
			NewVM = NewObject<UBooleanSettingViewModel>(this);
			break;
			default:
			break;
		}

		if (NewVM)
		{
			NewVM->SetDisplayName(Row->DisplayName);
			NewVM->SetInteractionType(Row->InteractionType);

			NewVM->BaseHandler = NewObject<USettingHandlerBase>(NewVM, Row->CustomHandlerClass);

			if (Row->SettingCategory == FName("Video"))
			{
				VideoSettings.Add(NewVM);
			}
			else if (Row->SettingCategory == FName("Audio"))
			{
				AudioSettings.Add(NewVM);
			}
		}
	}
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(VideoSettings);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(AudioSettings);
}

void USettingsManagerViewModel::SubMenuRegister(FName SubName, UDataTable* SubTable)
{
}
