// Copyright Preetham Mukundan (C) 2026

#include "Mover/ZeroSlideMode.h"
#include "MoveLibrary/MovementUtils.h"
#include "MoverComponent.h"
#include "Engine/World.h"
#include "Mover/ZeroMovementData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ZeroSlideMode)

UZeroSlideMode::UZeroSlideMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayTags.AddTag(Mover_IsOnGround);
	SlideInitialSpeedBoost = 500.0f;
	SlideGravityForce = 1200.0f;
	SlideSteeringAuthority = 2500.0f; 
	SlideFriction = 0.5f; 
	SlideMinSpeed = 150.0f;
	SlideJumpImpulse = 650.0f;
}

void UZeroSlideMode::GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{
	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	
	OutProposedMove.bHasDirIntent = true;
	if (StartingSyncState)
	{
		OutProposedMove.LinearVelocity = StartingSyncState->GetVelocity_WorldSpace();
	}
	else
	{
		OutProposedMove.LinearVelocity = FVector::ZeroVector;
	}
	
	OutProposedMove.AngularVelocityDegrees = FVector::ZeroVector;
}

void UZeroSlideMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;

	const FZeroMovementInputs* Inputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>();
	const FCharacterDefaultInputs* DefaultInputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	const FMoverDefaultSyncState* StartingSyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();

	if (!StartingSyncState || !Params.MovingComps.UpdatedComponent.IsValid())
	{
		OutputState.MovementEndState.NextModeName = DefaultModeNames::Walking;
		return;
	}

	USceneComponent* UpdatedComponent = Params.MovingComps.UpdatedComponent.Get();
	
	if (DefaultInputs && DefaultInputs->bIsJumpJustPressed)
	{
		OutputState.MovementEndState.NextModeName = DefaultModeNames::Falling;
		FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
		
		FVector JumpVelocity = StartingSyncState->GetVelocity_WorldSpace();
		JumpVelocity.Z = SlideJumpImpulse;

		OutputSyncState.SetTransforms_WorldSpace(UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentRotation(), JumpVelocity, FVector::ZeroVector, nullptr);
		UpdatedComponent->ComponentVelocity = JumpVelocity;
		return;
	}

	OutputState.SyncState.MovementMode = Params.StartState.SyncState.MovementMode;
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	
	FVector CurrentVelocity = StartingSyncState->GetVelocity_WorldSpace();
	
	if (Params.StartState.SyncState.MovementMode != "Sliding")
	{
		FVector BoostDir = CurrentVelocity.GetSafeNormal2D();
		if (BoostDir.IsNearlyZero() && Inputs)
		{
			BoostDir = Inputs->LookDir.Vector().GetSafeNormal2D();
		}
		
		CurrentVelocity += BoostDir * SlideInitialSpeedBoost;
	}


	FRotator TargetOrient = UpdatedComponent->GetComponentRotation();
	if (Inputs)
	{
		TargetOrient = Inputs->LookDir.Vector().ToOrientationRotator();
	}
	TargetOrient.Pitch = 0.0f;
	TargetOrient.Roll = 0.0f;
	FQuat NewRot = TargetOrient.Quaternion();
	
	FVector CumulativeForces = FVector::DownVector * SlideGravityForce;

	if (Inputs && !Inputs->MoveInput.IsNearlyZero())
	{
		FVector RightVec = FRotationMatrix(TargetOrient).GetUnitAxis(EAxis::Y);
		float SteeringDot = FVector::DotProduct(Inputs->MoveInput.GetSafeNormal2D(), RightVec);
		CumulativeForces += RightVec * (SteeringDot * SlideSteeringAuthority);
	}
	
	if (Inputs && CurrentVelocity.Size2D() > 10.0f)
	{
		FVector LookDir2D = Inputs->LookDir.Vector().GetSafeNormal2D();
		float Speed2D = CurrentVelocity.Size2D();
		FVector BlendedDir = FMath::VInterpTo(CurrentVelocity.GetSafeNormal2D(), LookDir2D, DeltaSeconds, 3.0f);
		CurrentVelocity = BlendedDir.GetSafeNormal2D() * Speed2D + FVector(0, 0, CurrentVelocity.Z);
	}

	CurrentVelocity += CumulativeForces * DeltaSeconds;

	
	float Speed = CurrentVelocity.Size2D();
	if (Speed > 0.0f)
	{
		float FrictionDrop = SlideFriction * 500.0f * DeltaSeconds; 
		float NewSpeed = FMath::Max(Speed - FrictionDrop, 0.0f);
		CurrentVelocity = CurrentVelocity.GetSafeNormal2D() * NewSpeed + FVector(0, 0, CurrentVelocity.Z);
	}


	FVector MoveDelta = CurrentVelocity * DeltaSeconds;
	FHitResult Hit(1.f);
	FMovementRecord MoveRecord;
	MoveRecord.SetDeltaSeconds(DeltaSeconds);

	UMovementUtils::TrySafeMoveUpdatedComponent(Params.MovingComps, MoveDelta, NewRot, true, Hit, ETeleportType::None, MoveRecord);

	if (Hit.bBlockingHit && Hit.Time < 1.f)
	{
		FVector RemainingDelta = MoveDelta * (1.f - Hit.Time);
		FVector SlideDelta = FVector::VectorPlaneProject(RemainingDelta, Hit.Normal);
		
		FHitResult SlideHit(1.f);
		UMovementUtils::TrySafeMoveUpdatedComponent(Params.MovingComps, SlideDelta, NewRot, true, SlideHit, ETeleportType::None, MoveRecord);
		CurrentVelocity = FVector::VectorPlaneProject(CurrentVelocity, Hit.Normal);
	}


	if (Hit.bBlockingHit && Hit.Normal.Z > 0.01f && Hit.Normal.Z < 0.99f)
	{
		FVector SlopeDownwardDir = FVector::VectorPlaneProject(FVector::DownVector, Hit.Normal).GetSafeNormal();

		if (FVector::DotProduct(CurrentVelocity, SlopeDownwardDir) > 0.0f)
		{
			CurrentVelocity += SlopeDownwardDir * (SlideGravityForce * 1.5f) * DeltaSeconds;
		}
	}


	const bool bWantsToCrouch = Inputs && Inputs->bWantsToCrouch;
	if (!bWantsToCrouch || CurrentVelocity.SizeSquared2D() < FMath::Square(SlideMinSpeed))
	{
		OutputState.MovementEndState.NextModeName = DefaultModeNames::Walking;
	}

	OutputSyncState.SetTransforms_WorldSpace(UpdatedComponent->GetComponentLocation(),UpdatedComponent->GetComponentRotation(),CurrentVelocity, FVector::ZeroVector, nullptr
	);
	
	UpdatedComponent->ComponentVelocity = CurrentVelocity;
}