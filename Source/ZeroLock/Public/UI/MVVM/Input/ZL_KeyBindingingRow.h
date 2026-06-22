// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_KeyBindingingRow.generated.h"

class UZL_VM_KeyBindingRow;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_KeyBindingingRow : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void SetRowViewModel(UZL_VM_KeyBindingRow* InViewModel);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UZL_VM_KeyBindingRow* VM_Attributes;
};
