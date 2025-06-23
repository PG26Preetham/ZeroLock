// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZeroBase_HeavyMelee.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZeroBase_HeavyMelee : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UZeroBase_HeavyMelee();
	
	UFUNCTION()
	void MeleeDistanceFinished();

	
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	class UGAST_MeleeMoveTo* MeleeMoveTask;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MeleeTime = 2;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MeleeSpeed = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> HeavyMeleeDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ParryEffect;
	
};
