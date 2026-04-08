// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Input/ZL_KeyBindingScreen.h"

#include "Components/DynamicEntryBox.h"
#include "UI/MVVM/Input/ZL_KeyBindingingRow.h"

void UZL_KeyBindingScreen::OnKeybindRowsUpdated(const TArray<UZL_VM_KeyBindingRow*>& NewRows)
{
	if (!KeybindEntryBox ) return;


	KeybindEntryBox->Reset(true);
	
	for (UZL_VM_KeyBindingRow* row : NewRows)
	{
		if (UZL_KeyBindingingRow* NewEntry = KeybindEntryBox->CreateEntry<UZL_KeyBindingingRow>())
		{
			NewEntry->SetRowViewModel(row);
		}
	}
}
