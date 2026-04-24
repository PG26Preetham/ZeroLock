// Copyright Preetham Mukundan (C) 2026


#include "Mover/ZeroSlideMode.h"
#include "MoveLibrary/MovementUtils.h"
#include "MoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "Mover/ZeroMovementData.h"
#include "Mover/ZeroMoverPawn.h"


bool UZeroSlideMode::GetSlideSurface(const FSimulationTickParams& Params, FHitResult& OutHit) const
{
	// 1. Ensure the component is valid
	if (!Params.MovingComps.UpdatedComponent.IsValid()) return false;

	// 2. Cast the generic UpdatedComponent to a CapsuleComponent
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Params.MovingComps.UpdatedComponent.Get());
	if (!Capsule) return false; // Safety abort if the root isn't a capsule

	// 3. Perform the math
	FVector Start = Params.MovingComps.UpdatedComponent->GetComponentLocation();
	float CapHH = Capsule->GetScaledCapsuleHalfHeight();
	FVector End = Start + (CapHH * 2.0f * FVector::DownVector);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Params.MovingComps.UpdatedComponent->GetOwner());

	return Params.MovingComps.UpdatedComponent->GetWorld()->LineTraceSingleByProfile(
		OutHit, Start, End, TEXT("BlockAllDynamic"), TraceParams
	);
}

void UZeroSlideMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;

	const FZeroMovementInputs* Inputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>();
	const FMoverDefaultSyncState* StartingSyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();

	const UZeroMovementSettings* Settings = nullptr;
	if (AZeroMoverPawn* Pawn = Cast<AZeroMoverPawn>(GetMoverComponent()->GetOwner()))
	{
		Settings = Pawn->SlideSettings;
	}

	if (!Settings || !StartingSyncState)
	{
		OutputState.MovementEndState.NextModeName = DefaultModeNames::Walking;
		return;
	}

	OutputState.SyncState.MovementMode = Params.StartState.SyncState.MovementMode;
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();

	FVector OldLocation = Params.MovingComps.UpdatedComponent->GetComponentLocation();
	FVector CurrentVelocity = StartingSyncState->GetVelocity_WorldSpace();


	if (Params.StartState.SyncState.MovementMode != TEXT("Sliding"))
	{
		CurrentVelocity += CurrentVelocity.GetSafeNormal2D() * Settings->SlideInitialSpeedBoost;
	}

	CurrentVelocity += FVector::DownVector * Settings->SlideGravityForce * DeltaSeconds;

	FVector SteeringAcceleration = FVector::ZeroVector;
	if (Inputs && !Inputs->MoveInput.IsNearlyZero())
	{
		FVector RightVec = Params.MovingComps.UpdatedComponent->GetRightVector();
		
		if (FMath::Abs(FVector::DotProduct(Inputs->MoveInput.GetSafeNormal(), RightVec)) > 0.5f)
		{
			SteeringAcceleration = Inputs->MoveInput.ProjectOnTo(RightVec) * 2048.0f;
		}
	}

	float Speed = CurrentVelocity.Size2D();
	if (Speed > 0.0f)
	{
		float Drop = Speed * Settings->SlideFriction * DeltaSeconds;
		float NewSpeed = FMath::Max(Speed - Drop, 0.0f);
		CurrentVelocity = CurrentVelocity.GetSafeNormal2D() * NewSpeed + FVector(0, 0, CurrentVelocity.Z);
	}
	CurrentVelocity += SteeringAcceleration * DeltaSeconds;


	FVector VelPlaneDir = CurrentVelocity.GetSafeNormal2D();
	FQuat NewRot = FRotationMatrix::MakeFromXZ(VelPlaneDir, FVector::UpVector).ToQuat();

	FVector MoveDelta = CurrentVelocity * DeltaSeconds;
	FHitResult Hit(1.f);
	FMovementRecord MoveRecord;
	MoveRecord.SetDeltaSeconds(DeltaSeconds);

	UMovementUtils::TrySafeMoveUpdatedComponent(Params.MovingComps, MoveDelta, NewRot, true, Hit, ETeleportType::None, MoveRecord);

	if (Hit.bBlockingHit && Hit.Time < 1.f)
	{
		FVector RemainingDelta = MoveDelta * (1.f - Hit.Time);
		FVector SlideDelta = FVector::VectorPlaneProject(RemainingDelta, Hit.Normal);
		
		FQuat SlopeRot = FRotationMatrix::MakeFromXZ(SlideDelta.GetSafeNormal2D(), Hit.Normal).ToQuat();
		FHitResult SlideHit(1.f);
		UMovementUtils::TrySafeMoveUpdatedComponent(Params.MovingComps, SlideDelta, SlopeRot, true, SlideHit, ETeleportType::None, MoveRecord);
	}


	FVector FinalVelocity = (Params.MovingComps.UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaSeconds;


	if (Hit.bBlockingHit && Hit.Normal.Z > 0.7f)
	{

		FinalVelocity = FinalVelocity.GetSafeNormal() * CurrentVelocity.Size();
	}


	bool bWantsToCrouch = Inputs && Inputs->bWantsToCrouch;
	
	
	if (!bWantsToCrouch || FinalVelocity.SizeSquared2D() < FMath::Square(Settings->SlideMinSpeed))
	{
		OutputState.MovementEndState.NextModeName = DefaultModeNames::Walking;
	}

	OutputSyncState.SetTransforms_WorldSpace(
		Params.MovingComps.UpdatedComponent->GetComponentLocation(),
		Params.MovingComps.UpdatedComponent->GetComponentRotation(),
		FinalVelocity, 
		nullptr
	);
}
