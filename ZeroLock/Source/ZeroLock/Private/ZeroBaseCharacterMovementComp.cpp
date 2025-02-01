// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroBaseCharacterMovementComp.h"

#include "MaterialHLSLTree.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZeroBaseCharacterMovementComp::UZeroBaseCharacterMovementComp()
{
	NavAgentProps.bCanCrouch=true;
}

void UZeroBaseCharacterMovementComp::SprintPressed()
{
	Safe_bWantSprint = true;
}

void UZeroBaseCharacterMovementComp::SprintReleased()
{
	Safe_bWantSprint = false;
}

void UZeroBaseCharacterMovementComp::CrouchPressed()
{
	bWantsToCrouch = true;
}

void UZeroBaseCharacterMovementComp::CrouchReleased()
{
	bWantsToCrouch = false;
}

bool UZeroBaseCharacterMovementComp::IsCustomMovementMode(ECustomMovementModes InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

void UZeroBaseCharacterMovementComp::InitializeComponent()
{
	Super::InitializeComponent();

	ZeroCharacterOwner = Cast<AZeroLockCharacter>(GetOwner());
}

void UZeroBaseCharacterMovementComp::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity+=Velocity.GetSafeNormal2D() * SlideEnterImpluse;
	SetMovementMode(MOVE_Custom,CMove_Slide);
}

void UZeroBaseCharacterMovementComp::ExitSlide()
{
	bWantsToCrouch = false;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(),FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector,NewRotation,true,Hit);
	SetMovementMode(MOVE_Walking);
}

void UZeroBaseCharacterMovementComp::PhysSlide(float deltaTime, int32 Iterations)
{
	if(deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if(!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(SlideMinSpeed,2))
	{
		//Exit slide
		ExitSlide();

		//Start new physics function which is not slide Phys
		StartNewPhysics(deltaTime, Iterations);
		return;
	}
	
	//Apply surface gravity
	Velocity+= SlideGravityForce * FVector::DownVector *deltaTime;

	//strafe
	if(FMath::Abs( FVector::DotProduct(Acceleration.GetSafeNormal() , UpdatedComponent->GetRightVector()) ) > 0.5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	//Calculate Velocity
	if(!HasAnimRootMotion() && CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime,SlideFriction,true,GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	//Perform Slide
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotator = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity,SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir,SurfaceHit.Normal).ToQuat();

	//This function moves the character
	SafeMoveUpdatedComponent(Adjusted,NewRotation,true,Hit);

	if(Hit.Time < 1.0f)
	{
		HandleImpact(Hit,deltaTime,Adjusted);
		SlideAlongSurface(Adjusted,1-Hit.Time,Hit.Normal,Hit,true);
	}

	FHitResult NewSurfaceHit;
	if(!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(SlideMinSpeed,2))
	{
		ExitSlide();
	}

	//Update outgoing velocity
	if(!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent ->GetComponentLocation() - OldLocation)/deltaTime;
	}
}

bool UZeroBaseCharacterMovementComp::GetSlideSurface(FHitResult& OutHit) const
{
	FVector Start  = UpdatedComponent->GetComponentLocation();
	FVector End    = Start + GetCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2 *FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(OutHit,Start,End,ProfileName,ZeroCharacterOwner->GetIgnoreCharacterParams());
}

void UZeroBaseCharacterMovementComp::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantSprint)
		{
			MaxWalkSpeed = Max_SprintSpeed;
		}
		else
		{
			MaxWalkSpeed = Max_WalkSpeed;
		}
	}
	Safe_bPrevWantsToCrouch = bWantsToCrouch;
}

void UZeroBaseCharacterMovementComp::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
if(MovementMode == MOVE_Walking && !bWantsToCrouch && Safe_bPrevWantsToCrouch)
{
	FHitResult  PotentialSlideSurface;
	if(Velocity.SizeSquared() < pow(SlideMinSpeed,2) && GetSlideSurface(PotentialSlideSurface))
	{
		EnterSlide();
	}
}
	if(IsCustomMovementMode(CMove_Slide) && !bWantsToCrouch)
	{
		ExitSlide();
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UZeroBaseCharacterMovementComp::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case CMove_Slide:
			PhysSlide(deltaTime, Iterations);
			break;
		default:
			UE_LOG(LogTemp,Fatal ,TEXT("Invalid Custom Movement Mode"));
	}
}

bool UZeroBaseCharacterMovementComp::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMove_Slide);
}

bool UZeroBaseCharacterMovementComp::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() || IsMovingOnGround();
}


UZeroBaseCharacterMovementComp::FSavedMove_Zero::FSavedMove_Zero()
{
}

bool UZeroBaseCharacterMovementComp::FSavedMove_Zero::CanCombineWith(const FSavedMovePtr& NewMove,ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Zero* NewZeroSaved = static_cast<FSavedMove_Zero*>(NewMove.Get());
	if(NewZeroSaved != nullptr)
	{
		if(NewZeroSaved->S_bWantsSprint != S_bWantsSprint)
		{
			return false;
		}
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::Clear()
{
	FSavedMove_Character::Clear();
	S_bWantsSprint = 0;
}

uint8 UZeroBaseCharacterMovementComp::FSavedMove_Zero::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if(S_bWantsSprint) Result |= FLAG_Custom_0;
	
	return Result;
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::SetMoveFor(ACharacter* C, float InDeltaTime,FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UZeroBaseCharacterMovementComp* CharacterMovement = Cast<UZeroBaseCharacterMovementComp>(C->GetCharacterMovement());

	S_bWantsSprint = CharacterMovement->Safe_bWantSprint;
	Saved_bPrevWantsToCrouch = CharacterMovement->Safe_bPrevWantsToCrouch;
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	
	UZeroBaseCharacterMovementComp* CharacterMovement = Cast<UZeroBaseCharacterMovementComp>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantSprint = S_bWantsSprint;
	CharacterMovement->Safe_bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;
}

UZeroBaseCharacterMovementComp::FNetworkPredictionData_Client_Zero::FNetworkPredictionData_Client_Zero(
	const UCharacterMovementComponent& ClientMovement) :Super(ClientMovement)
{

}

FSavedMovePtr UZeroBaseCharacterMovementComp::FNetworkPredictionData_Client_Zero::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Zero());
}

void UZeroBaseCharacterMovementComp::UpdateFromCompressedFlags(uint8 Flags)
{
	
	Super::UpdateFromCompressedFlags(Flags);
	Safe_bWantSprint =(Flags & FSavedMove_Zero::FLAG_Custom_0) != 0;

}

class FNetworkPredictionData_Client* UZeroBaseCharacterMovementComp::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if(ClientPredictionData == nullptr)
	{
		UZeroBaseCharacterMovementComp* MutableThis = const_cast<UZeroBaseCharacterMovementComp*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Zero(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist =140.f;
	}
	return ClientPredictionData;
}
