// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZeroLock/ZeroLock.h"
#include "BaseGameplayAbility.generated.h"

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
};
