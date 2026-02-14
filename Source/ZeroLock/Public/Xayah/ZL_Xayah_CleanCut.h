// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Xayah_CleanCut.generated.h"

class AZL_Xayah_projectile;
class AZero_BaseProjectile;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Xayah_CleanCut : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UZL_Xayah_CleanCut();

	UPROPERTY()
	TArray<TWeakObjectPtr<AZL_Xayah_projectile>> ActiveFeathers;

	UFUNCTION()
	virtual void OnEventRecived(FGameplayEventData Payload);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
