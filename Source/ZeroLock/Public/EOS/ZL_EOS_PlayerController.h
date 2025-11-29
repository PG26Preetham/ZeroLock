// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZL_EOS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_EOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZL_EOS_PlayerController();

	virtual void BeginPlay() override;

	void Login();

	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	FDelegateHandle LoginDelegateHandle;
	
};
