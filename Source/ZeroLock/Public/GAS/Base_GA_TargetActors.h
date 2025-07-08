// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "Base_GA_TargetActors.generated.h"

class AGATA_SingleLineTrace;
/**
 * 
 */
class AGameplayAbilityTargetActor;
UCLASS()
class ZEROLOCK_API UBase_GA_TargetActors : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UBase_GA_TargetActors();


	UFUNCTION()
	void TargetConfirmed(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void TargetCancelled(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void TargetInputRelease(float TimeHeld);
	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	

	virtual void AbilityConfirmedAction(const FGameplayAbilityTargetDataHandle& Data);
	virtual void AbilityCancelledAction(const FGameplayAbilityTargetDataHandle& Data);
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<AGameplayAbilityTargetActor> Targetclass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	TSubclassOf<class AGameplayAbilityWorldReticle> ReticleClassX;
	
	class	UAbilityTask_WaitTargetData* WaitTargetTask;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	EGASTargetConfirmationStyle TargetConfirmationStyle;
};
