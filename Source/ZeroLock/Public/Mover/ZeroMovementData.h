// ZeroMovementData.h
#pragma once

#include "CoreMinimal.h"
#include "MoverDataModelTypes.h"
#include "Engine/DataAsset.h"
#include "ZeroMovementData.generated.h"


USTRUCT(BlueprintType)
struct FZeroMovementInputs : public FMoverDataStructBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	FVector MoveInput = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	bool bWantsToCrouch = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	bool bSlideIntentValid = true;
	
	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	bool bCustomJumpJustPressed = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	bool bWantsToAirJump = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Zero|Inputs")
	bool bWantsToDash = false;

	virtual UScriptStruct* GetScriptStruct() const override { return FZeroMovementInputs::StaticStruct(); }
	virtual FMoverDataStructBase* Clone() const override { return new FZeroMovementInputs(*this); }

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);
		Ar << MoveInput;
		Ar.SerializeBits(&bWantsToCrouch, 1);
		Ar.SerializeBits(&bSlideIntentValid, 1); 
		Ar.SerializeBits(&bCustomJumpJustPressed, 1); 
		Ar.SerializeBits(&bWantsToAirJump, 1);
		Ar.SerializeBits(&bWantsToDash, 1);
		bOutSuccess = true;
		return true;
	}
};

USTRUCT(BlueprintType)
struct FZeroSlideState : public FMoverDataStructBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Zero|State")
	FVector SlideDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Zero|State")
	float TimeSpentSliding = 0.0f;
	
	virtual UScriptStruct* GetScriptStruct() const override { return FZeroSlideState::StaticStruct(); }

	virtual FMoverDataStructBase* Clone() const override { return new FZeroSlideState(*this); }
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);
		Ar << SlideDirection;
		Ar << TimeSpentSliding;
		bOutSuccess = true;
		return true;
	}

	virtual bool ShouldReconcile(const FMoverDataStructBase& AuthorityState) const override
	{
		const FZeroSlideState* AuthState = static_cast<const FZeroSlideState*>(&AuthorityState);
		return !SlideDirection.Equals(AuthState->SlideDirection, 0.01f) || 
			   FMath::Abs(TimeSpentSliding - AuthState->TimeSpentSliding) > 0.05f;
	}

	virtual void Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct) override
	{
		const FZeroSlideState* FromState = static_cast<const FZeroSlideState*>(&From);
		const FZeroSlideState* ToState = static_cast<const FZeroSlideState*>(&To);
		SlideDirection = FMath::Lerp(FromState->SlideDirection, ToState->SlideDirection, Pct);
		TimeSpentSliding = FMath::Lerp(FromState->TimeSpentSliding, ToState->TimeSpentSliding, Pct);
	}
};


UCLASS(BlueprintType)
class ZEROLOCK_API UZeroMovementSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slide")
	float SlideMinSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slide")
	float SlideInitialSpeedBoost = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slide")
	float SlideFriction = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slide")
	float MaxSlideDuration = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slide")
	float SlideGravityForce = 10.f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air Jump")
	int32 MaxAirJumps = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air Jump")
	float AirJumpForce = 600.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air Jump")
	float AirJumpLateralBoost = 300.0f; 

	// WALL JUMP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall Jump")
	float WallJumpOffForce = 500.0f; // Horizontal push away from the wall

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall Jump")
	float WallJumpVerticalForce = 700.0f; // Vertical upward push
};