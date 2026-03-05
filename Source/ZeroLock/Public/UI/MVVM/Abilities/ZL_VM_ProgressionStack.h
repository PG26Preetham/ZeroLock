// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_ProgressionStack.generated.h"

class UZL_VM_AbilityTimerProgressBar;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_ProgressionStack : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	TMap<FName,UZL_VM_AbilityTimerProgressBar*> ProgressionMeters;
	
	
	TMap<FName,UZL_VM_AbilityTimerProgressBar*> GetProgressionMeters()const{ return ProgressionMeters; };
	
	void SetProgressionMeters(TMap<FName,UZL_VM_AbilityTimerProgressBar*> Meters);
	
	void AddProgressMeter(UZL_VM_AbilityTimerProgressBar* Meter);
	
	void RemoveProgressMeter(UZL_VM_AbilityTimerProgressBar* Meter);
	

	
};
