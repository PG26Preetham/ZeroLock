// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Xayah_DeadlyPlumage.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Xayah_DeadlyPlumage : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UZL_Xayah_DeadlyPlumage();

virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> FeatherDownClassClass;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> BuffClass;
};
