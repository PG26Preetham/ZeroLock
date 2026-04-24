// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MovementMode.h"
#include "ZeroSlideMode.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZeroSlideMode : public UBaseMovementMode
{
	GENERATED_BODY()
	
protected:
	bool GetSlideSurface(const FSimulationTickParams& Params, FHitResult& OutHit) const;
	
	virtual void SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
};
