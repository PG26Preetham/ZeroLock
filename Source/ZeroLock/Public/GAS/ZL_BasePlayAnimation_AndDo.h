// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_BasePlayAnimation_AndDo.generated.h"

class UGAST_PlayMontageAndWaitForEvent;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_BasePlayAnimation_AndDo : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UZL_BasePlayAnimation_AndDo();

	UFUNCTION()
	void OnCanelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void OnAnimationPointTrigger();

	virtual void OnAnimationCancelled();

	UGAST_PlayMontageAndWaitForEvent* PlayMontageTask;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	FGameplayTagContainer EventTagToWait;
	
};
