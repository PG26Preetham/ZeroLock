// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroBaseCharacterMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZeroBaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Zero : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

	public:
		FSavedMove_Zero() ;
	
		
		uint8 S_bWantsSprint:1;

		//Check if the previous and current saveddata can be combined so we can save bandwidth
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

		
		virtual void Clear() override;

		//compress bools into bits
		virtual uint8 GetCompressedFlags() const override;

		//Set Saved data from character
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)override;
		//Set Character from saved data
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Zero : public FNetworkPredictionData_Client_Character
	{
		public:
		FNetworkPredictionData_Client_Zero(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
		
	};

	
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	
	bool Safe_bWantSprint;
	
public:
	UZeroBaseCharacterMovementComp();

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	//Sprint
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
	

	UPROPERTY(EditDefaultsOnly)
	float Max_SprintSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Max_WalkSpeed;
};


