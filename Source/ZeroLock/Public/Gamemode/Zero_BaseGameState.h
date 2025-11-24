// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Zero_BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamRedScore = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamBlueScore = 0;

	void AddKill(int32 TeamID);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
