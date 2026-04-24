// Copyright Preetham Mukundan (C) 2026


#include "Mover/Transitions/ZeroSlideTransition.h"

#include "MoverSimulationTypes.h"
#include "Mover/ZeroMovementData.h"
#include "Mover/ZeroMoverPawn.h"

FTransitionEvalResult UZeroSlideTransition::Evaluate_Implementation(const FSimulationTickParams& Params) const
{
	FTransitionEvalResult EvalResult = FTransitionEvalResult::NoTransition;

	const FZeroMovementInputs* Inputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>();
	const FMoverDefaultSyncState* SyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();

	const UZeroMovementSettings* Settings = nullptr;
	if (AZeroMoverPawn* Pawn = Cast<AZeroMoverPawn>(Params.MovingComps.MoverComponent->GetOwner()))
	{
		Settings = Pawn->SlideSettings;
	}

	if (!Inputs || !SyncState || !Settings) return EvalResult;

	FVector CurrentVelocity = SyncState->GetVelocity_WorldSpace();


	float RequiredEntrySpeed = Settings->SlideMinSpeed + 150.0f; 

	
	if (Inputs->bWantsToCrouch && 
		Inputs->bSlideIntentValid && 
		Params.StartState.SyncState.MovementMode == DefaultModeNames::Walking && 
		CurrentVelocity.SizeSquared2D() > FMath::Square(RequiredEntrySpeed)) 
	{
		EvalResult.NextMode = TEXT("Sliding");
	}

	return EvalResult;
}
