// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_KeyBindingList.generated.h"

class UZL_VM_KeyBindingRow;
class UKeybindManagerSubsystem;

UCLASS()
class ZEROLOCK_API UZL_VM_KeyBindingList : public UMVVMViewModelBase
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Keybindings")
	TArray<UZL_VM_KeyBindingRow*> KeybindRows;

	TArray<UZL_VM_KeyBindingRow*> GetKeybindRows() const { return KeybindRows; }


	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void InitializeWithPlayer(ULocalPlayer* LocalPlayer);

	
	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void RequestKeyRemap(UZL_VM_KeyBindingRow* RowVM, FKey NewKey);

private:
	UPROPERTY()
	UKeybindManagerSubsystem* SubsystemRef;

	UFUNCTION()
	void HandleSubsystemBindingsUpdated();
};
