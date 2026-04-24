// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"
#include "ZeroSlideTransition.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZeroSlideTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Sliding")
	FName SlidingModeName;

	virtual FTransitionEvalResult Evaluate_Implementation(const FSimulationTickParams& Params) const override;
};
