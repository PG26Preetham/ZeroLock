// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BasePassive_GameplayAbility.h"
#include "ZL_Base_Passive_ApplyToSelf.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Base_Passive_ApplyToSelf : public UBasePassive_GameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnEventRecived(FGameplayEventData Payload) override;
	
};
