// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroObjective.generated.h"

class AZeroLockCharacter;

UCLASS()
class ZEROLOCK_API AZeroObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZeroObjective();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Objective")
	TObjectPtr<USkeletalMeshComponent> ObjectiveMesh;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Objective")
	TObjectPtr<AZeroLockCharacter> PlayerTarget = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
