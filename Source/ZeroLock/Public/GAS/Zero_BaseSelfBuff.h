//Copyright Preetham Mukundan (C) 2026

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
	float TimeBeforeBuff = 0.1;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> BuffOrDebufsToApply;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION()
	void OnFinishedDelay();
};
