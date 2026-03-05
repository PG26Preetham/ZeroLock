// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_AbilityProgressBar_Stack.generated.h"

class UZL_VM_AbilityTimerProgressBar;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_AbilityProgressBar_Stack : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateTheList(TMap<FName,UZL_VM_AbilityTimerProgressBar*> NewList);
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UCommonListView* ProgressBarList;
	
};
