// Fill out your copyright notice in the Description page of Project Settings.

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
