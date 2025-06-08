// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "Zero_BaseWeaponAbility.generated.h"

class AZero_BaseProjectile;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZero_BaseWeaponAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UZero_BaseWeaponAbility();
	
	virtual void Fire();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AZero_BaseProjectile> ProjectileClass;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	

	
};
