// Copyright Preetham Mukundan (C) 2026


#include "Mover/Transitions/ZeroSlideTransition.h"

#include "MoverSimulationTypes.h"
#include "Mover/ZeroMovementData.h"
#include "Mover/ZeroMoverPawn.h"

UZeroSlideTransition::UZeroSlideTransition(const FObjectInitializer& ObjectInitializer)
{
}

FTransitionEvalResult UZeroSlideTransition::Evaluate_Implementation(const FSimulationTickParams& Params) const
{
	FTransitionEvalResult EvalResult = FTransitionEvalResult::NoTransition;

	const UCharacterMoverComponent* MoverComp = Cast<UCharacterMoverComponent>(Params.MovingComps.MoverComponent.Get());
	const FMoverDefaultSyncState* SyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	const FZeroMovementInputs* Inputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>();
	
	if (MoverComp && MoverComp->IsOnGround() && Params.StartState.SyncState.MovementMode != SlidingModeName)
	{
		if (Inputs && SyncState)
		{
			const bool bWantsToCrouch = Inputs->bWantsToCrouch;
			const bool bIsMovingFastEnough = SyncState->GetVelocity_WorldSpace().SizeSquared2D() >= FMath::Square(MinSpeedToSlide);

			if (bWantsToCrouch && bIsMovingFastEnough)
			{
				EvalResult.NextMode = SlidingModeName;
			}
		}
	}

	return EvalResult;
}
