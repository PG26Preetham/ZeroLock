// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "BasePassive_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UBasePassive_GameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()


	UBasePassive_GameplayAbility();

	public:
	UFUNCTION()
	void OnEventRecived(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	class UAbilityTask_WaitGameplayEvent* WaitGameplayEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayEffect> PassiveEffectToApply;
};
