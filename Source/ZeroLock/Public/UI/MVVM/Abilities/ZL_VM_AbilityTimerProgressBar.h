// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_AbilityTimerProgressBar.generated.h"


UCLASS()
class ZEROLOCK_API UZL_VM_AbilityTimerProgressBar : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	FText AbilityName;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	float ProgressionLevel;
	
	FText GetAbilityName() const { return AbilityName; }
	float GetProgressionLevel() const { return ProgressionLevel; }
	
	void SetProgressionLevel(float level);
	void SetAbilityName(FText name);
};
