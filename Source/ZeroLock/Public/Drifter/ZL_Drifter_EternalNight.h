//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Drifter_EternalNight.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Drifter_EternalNight : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnWeaponEventTrigger(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	int32 MaxTargets = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> BlindEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> PlayerBuffEffectClass;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> CurrentActiveEffectHandles;


	UFUNCTION()
	void OnDelayFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Duration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float SpiritDamage = 15.0f;
};
