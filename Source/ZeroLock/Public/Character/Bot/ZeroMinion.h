// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ZeroMinion.generated.h"

UCLASS()
class ZEROLOCK_API AZeroMinion : public APawn
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

};
