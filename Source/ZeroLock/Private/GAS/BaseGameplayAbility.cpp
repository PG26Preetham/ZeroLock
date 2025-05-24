// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseGameplayAbility.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
}
