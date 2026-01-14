//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BasePlayAnimation_AndDo.h"
#include "ZL_Drifter_EternalNight.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Drifter_EternalNight : public UZL_BasePlayAnimation_AndDo
{
	GENERATED_BODY()

public:
	virtual void OnAnimationPointTrigger() override;
	virtual void OnAnimationCompleted() override;
	UFUNCTION()
	void OnWeaponEventTrigger(FGameplayEventData Payload);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat MaxTargets = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> BlindEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> PlayerBuffEffectClass;

	FActiveGameplayEffectHandle PlayerBuffEffectHandle;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> CurrentActiveEffectHandles;


	UFUNCTION()
	void OnDelayFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat Duration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FScalableFloat SpiritDamage = 15.0f;
};
