// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseCharAbilitySystemComponent.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewAbilityAddedDelegate,FGameplayAbilitySpec& ,AbilitySpec);
UCLASS()
class ZEROLOCK_API UBaseCharAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	UFUNCTION()
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;


	virtual void OnRep_ActivateAbilities() override;
public:
	UPROPERTY(BlueprintAssignable)
	FNewAbilityAddedDelegate OnNewAbilityAdded;
	
};
