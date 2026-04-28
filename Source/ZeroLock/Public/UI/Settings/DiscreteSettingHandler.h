// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/SettingHandlerBase.h"
#include "DiscreteSettingHandler.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZEROLOCK_API UDiscreteSettingHandler : public USettingHandlerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings Logic")
	void OnApplySelection(int32 SelectedIndex);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings Logic")
	int32 GetCurrentSelection();
	
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Settings Logic")
	TArray<FString> GetAllSelectionNames();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slections")
	TArray<FString> AllSelectionNames;
	
};
