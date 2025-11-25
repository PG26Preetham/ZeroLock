// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "ZeroMinion.generated.h"

class UCapsuleComponent;

UCLASS()
class ZEROLOCK_API AZeroMinion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZeroMinion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> MinionCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> MinionMesh;

};
