// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ZL_AbilityTimerBar.generated.h"

class UCommonTextBlock;
class UProgressBar;
class UZL_VM_AbilityTimerProgressBar;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_AbilityTimerBar : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void SetViewModel(UZL_VM_AbilityTimerProgressBar* InViewModel);
	
	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_AbilityTimerProgressBar* VM_Progress;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* AbilityName;
};
