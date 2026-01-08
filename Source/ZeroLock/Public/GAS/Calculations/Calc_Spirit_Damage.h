//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Calc_Spirit_Damage.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UCalc_Spirit_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UCalc_Spirit_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
