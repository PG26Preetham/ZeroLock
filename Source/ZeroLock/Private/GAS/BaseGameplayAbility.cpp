// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseGameplayAbility.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLLock.Abilities"),false));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
