// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Zero_BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AZero_BasePlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamID = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Kills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Deaths = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Assists = 0;

	void AddKill();
	void AddDeath();
	void AddAssist();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
