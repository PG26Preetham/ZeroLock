// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Input/ZL_VM_KeyBindingRow.h"

#include "Input/KeybindManagerSubsystem.h"
#include "ZeroLock/ZeroLock.h"

void UZL_VM_KeyBindingRow::SetSubsystemRef(UKeybindManagerSubsystem* SubsystemRefX)
{
	UE_MVVM_SET_PROPERTY_VALUE(SubsystemRef, SubsystemRefX);
}

void UZL_VM_KeyBindingRow::SetMappingName(FName MappingNameX)
{
	UE_MVVM_SET_PROPERTY_VALUE(MappingName, MappingNameX);
}

void UZL_VM_KeyBindingRow::SetActionName(FText InName)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionName, InName);
}

void UZL_VM_KeyBindingRow::SetCurrentKey(FInputChord InKey)
{

	UE_MVVM_SET_PROPERTY_VALUE(CurrentKey, InKey);
}

void UZL_VM_KeyBindingRow::ApplyNewKey(const FInputChord& SelectedKey)
{
	
	if (SubsystemRef )
	{
		SubsystemRef->RemapKey(MappingName, SelectedKey.Key);
	}
}
