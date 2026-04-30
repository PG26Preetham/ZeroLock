// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/Settings/ZL_SettingsSubMenu.h"
#include "ZL_KeyBindingScreen.generated.h"

class UDynamicEntryBox;
class UZL_VM_KeyBindingRow;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_KeyBindingScreen : public UZL_SettingsSubMenu
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* KeybindEntryBox;
	
	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void OnKeybindRowsUpdated(const TArray<UZL_VM_KeyBindingRow*>& NewRows);
	
};
