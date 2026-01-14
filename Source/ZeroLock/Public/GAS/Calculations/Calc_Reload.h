//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Calc_Reload.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UCalc_Reload : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UCalc_Reload();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	
};
