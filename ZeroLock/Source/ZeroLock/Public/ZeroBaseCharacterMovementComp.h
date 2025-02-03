// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroBaseCharacterMovementComp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ECustomMovementModes
{
	CMove_None UMETA(hidden),
	CMove_Slide UMETA(DisplayName = " Slide"),
	CMove_MAX UMETA(hidden)
};

UCLASS()
class ZEROLOCK_API UZeroBaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Zero : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

	public:
		FSavedMove_Zero() ;
	
		//flag
		uint8 S_bWantsSprint:1;

		uint8 Saved_bPrevWantsToCrouch:1;

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
	bool Safe_bPrevWantsToCrouch;
	
public:
	UZeroBaseCharacterMovementComp();

	//transient
	UPROPERTY(transient)
	class AZeroLockCharacter* ZeroCharacterOwner;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	//handles custom phys
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual bool CanAttemptJump() const override;

	//Sprint
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
	

	UPROPERTY(EditDefaultsOnly)
	float Max_SprintSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Max_WalkSpeed;


	//Crouch
	UFUNCTION(BlueprintCallable)
	void CrouchPressed();
	UFUNCTION(BlueprintCallable)
	void CrouchReleased();

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementModes InCustomMovementMode) const;

	protected:
	virtual void InitializeComponent() override;

	//slide
private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& OutHit) const;

	UPROPERTY(EditDefaultsOnly)
	float SlideMinSpeed =350;
	UPROPERTY(EditDefaultsOnly)
	float SlideEnterImpluse = 500;
	UPROPERTY(EditDefaultsOnly)
	float SlideGravityForce =5000;
	UPROPERTY(EditDefaultsOnly)
	float SlideFriction =1.3;
};


