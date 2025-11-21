// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseCharAbilitySystemComponent.h"

void UBaseCharAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (OnNewAbilityAdded.IsBound())
	{
		OnNewAbilityAdded.Broadcast(AbilitySpec);
	}
	
}
