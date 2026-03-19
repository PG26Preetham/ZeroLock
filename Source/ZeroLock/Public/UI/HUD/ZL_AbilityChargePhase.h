// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_AbilityChargePhase.generated.h"

class UZL_VM_ChargePercent;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_AbilityChargePhase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void SetViewModel(UZL_VM_ChargePercent* ViewModel);
	
	UPROPERTY()
	UZL_VM_ChargePercent* MyVM;
	
};
