// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BaseProjectileThrowAbility.h"
#include "ZL_GA_Drifter_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_GA_Drifter_Teleport : public UZL_BaseProjectileThrowAbility
{
	GENERATED_BODY()

public:
	
	virtual void OnEventRecived(FGameplayEventData Payload) override;

	class UAbilityTask_WaitInputPress* InputPTask;
	class UAbilityTask_WaitDelay* WaitDelay;

	const class AZeroLockCharacter* Villan;
	
	UFUNCTION()
	void OnInputPressed(float TimeWaited);
	UFUNCTION()
	void OnTimeFinish();
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayEffect> MarkEffectClass;

	
	FActiveGameplayEffectHandle AppliedEffectHandle;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag MarkedCueTag;
};
