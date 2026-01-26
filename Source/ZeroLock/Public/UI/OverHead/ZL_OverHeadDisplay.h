// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_OverHeadDisplay.generated.h"

class UZL_VM_Attributes;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_OverHeadDisplay : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void SetViewModel(UZL_VM_Attributes* InViewModel);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MVVM",meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_Attributes* VM_Attributes;
	
};
