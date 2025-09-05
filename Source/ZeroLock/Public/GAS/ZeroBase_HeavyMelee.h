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
	void OnFinish();
	UFUNCTION()
	void MeleeDistanceFinished();


	UFUNCTION()
	void WaitOver();
	UFUNCTION()
	void OnInputRelased(float TimeHeld);
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	class UGAST_MeleeMoveTo* MeleeMoveTask;

	class UGAST_PlayMontageAndWaitForEvent* MeleePlayMontageAndWaitForEvent;

	class UAbilityTask_WaitInputRelease* InputRTask;

	class UAbilityTask_WaitDelay* TaskWaitDelay;

	float MeleeMinHoldTime = 0.2f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MeleeTime = 2;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MeleeSpeed = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> HeavyMeleeDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> LightMeleeDamageEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ParryEffect;

	void MeleeDamageExec(TSubclassOf<class UGameplayEffect> MeleeDamageEffect,UAnimMontage* MeleeMontage,float KnockForce =0);
	
	void StartHeavyMelee();
	
};
