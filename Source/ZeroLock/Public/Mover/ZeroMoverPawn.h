// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "ZeroMovementData.h"
#include "GameFramework/Pawn.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "ZeroMoverPawn.generated.h"

class UZeroMovementSettings;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCapsuleComponent;

UCLASS()
class ZEROLOCK_API AZeroMoverPawn :public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	AZeroMoverPawn();
	UFUNCTION()
	void OnMoverStanceChanged(EStanceMode OldStance, EStanceMode NewStance);
	UFUNCTION()
	void OnMovementModeChanged(const FName& PreviousMovementModeName, const FName& NewMovementModeName);
	UFUNCTION()
	void OnPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd);
	
	
	UPROPERTY(Transient)
	FCharacterDefaultInputs DefaultInputData;
	
	UPROPERTY(Transient)
	FZeroMovementInputs ZeroInputsData;
	
	
	
	//Dash
	void HandleDashInputs();
	
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;
	
	
	// --- Core Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zero Mover|Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zero Mover|Components")
	USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zero Mover|Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zero Mover|Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zero Mover|Components")
	UCharacterMoverComponent* MoverComponent;

protected:
	// --- Enhanced Input Assets ---
	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputAction* CrouchAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Zero Mover|Input")
	UInputAction* DashAction;

	
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	
	//jump
	void OnJumpPressed();
	void OnJumpReleased();
	
	
	//crouch
	void OnCrouchPressed();
	void OnCrouchReleased();

public:
	// We put the settings directly on the Pawn!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement Settings")
	TObjectPtr<UZeroMovementSettings> SlideSettings;
private:
	bool bLocalJumpPressed = false;
	
	bool bCachedWantsToCrouch =false;
	
	bool bLocalSlideIntentValid = true;
	
	int32 LocalAirJumpsUsed = 0;
	bool bWasJumpPressedLastFrame = false;
	
protected:
	// Enhanced Input Callback
	void OnDashPressed();
	
	void TriggerDash(float DashSpeed, float DurationSeconds);

private:
	// Temporary hardware trigger latch
	bool bWantsToDashLatch = false;

private:
	// --- Cached Input State for the Mover Queue ---
	FVector2D CachedMoveInput;
};
