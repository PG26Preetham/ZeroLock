// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "Zero_BaseReloadAbility.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZero_BaseReloadAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UZero_BaseReloadAbility();

	UFUNCTION()
	void OnFinish();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ReloadEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ReloadingEffect;
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	class UAbilityTask_WaitDelay* TaskWaitDelay;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ReloadTime = 1;
};
