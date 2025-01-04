// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroBaseCharacterMovementComp.h"

#include "GameFramework/Character.h"

UZeroBaseCharacterMovementComp::UZeroBaseCharacterMovementComp()
{
}

void UZeroBaseCharacterMovementComp::SprintPressed()
{
	Safe_bWantSprint = true;
}

void UZeroBaseCharacterMovementComp::SprintReleased()
{
	Safe_bWantSprint = false;
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
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	
	UZeroBaseCharacterMovementComp* CharacterMovement = Cast<UZeroBaseCharacterMovementComp>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantSprint = S_bWantsSprint;
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
