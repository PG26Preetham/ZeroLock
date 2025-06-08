// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "Zero_BaseSelfBuff.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZero_BaseSelfBuff : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UZero_BaseSelfBuff();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> BuffOrDebufsToApply;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
