// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ZeroLock/ZeroLock.h"
#include "Zero_BasePlayerStart.generated.h"

UCLASS()
class ZEROLOCK_API AZero_BasePlayerStart : public APlayerStart
{
	GENERATED_BODY()



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeamID TeamID;
};
