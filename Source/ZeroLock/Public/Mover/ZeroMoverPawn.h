// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "ZeroMoverPawn.generated.h"

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

	
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	
	//jump
	void OnJumpPressed();
	void OnJumpReleased();

private:
	bool bLocalJumpPressed = false;

private:
	// --- Cached Input State for the Mover Queue ---
	FVector2D CachedMoveInput;
};
