//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Calc_Healing.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UCalc_Healing : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCalc_Healing();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
