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

public:

	UBasePassive_GameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UFUNCTION()
	virtual void OnEventRecived(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnHeroEventRecived(const FGameplayEventData& GameplayEventData);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayEffect> PassiveEffectToApply;
};
