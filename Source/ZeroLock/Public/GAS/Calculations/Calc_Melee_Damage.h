//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Calc_Melee_Damage.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UCalc_Melee_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCalc_Melee_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
