// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZeroLock/ZeroLock.h"
#include "BaseGameplayAbility.generated.h"

class APredictedProjectile;
class UBaseCharAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASTargetConfirmationStyle TargetStyle = EGASTargetConfirmationStyle::Instant;

public:
	UFUNCTION()
	void ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> GEToApply,UAbilitySystemComponent* TargetASC,UAbilitySystemComponent* SourceASC);

	UFUNCTION()
	void ApplyGameplyEffectToSelf(TSubclassOf<UGameplayEffect> GEToApply,UAbilitySystemComponent* SourseASC);

	UPROPERTY()
	APredictedProjectile* PredictedProjectile;
	
	UFUNCTION()
	UAbilitySystemComponent* GetOwnerASC();
};
