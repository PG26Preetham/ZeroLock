//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/Base_GA_TargetActors.h"
#include "ZL_GA_Seven_Stun.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_GA_Seven_Stun : public UBase_GA_TargetActors
{
	GENERATED_BODY()

	virtual void AbilityCancelledAction(const FGameplayAbilityTargetDataHandle& Data) override;
	virtual void AbilityConfirmedAction(const FGameplayAbilityTargetDataHandle& Data) override;

	class UAbilityTask_WaitDelay* StunTaskWaitDelay;

	
	TArray<TWeakObjectPtr<AActor>> OutActors;

	

public:
	UFUNCTION()
	void OnFinish();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> StunStartEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> StunEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	float StunDelay;
	
};
