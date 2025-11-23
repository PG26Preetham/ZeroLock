// Fill out your copyright notice in the Description page of Project Settings.


#include "Seven/ZL_GA_Seven_Stun.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Net/UnrealNetwork.h"
#include "Tasks/GameplayTask_WaitDelay.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_GA_Seven_Stun::AbilityCancelledAction(const FGameplayAbilityTargetDataHandle& Data)
{
	Super::AbilityCancelledAction(Data);
}

void UZL_GA_Seven_Stun::AbilityConfirmedAction(const FGameplayAbilityTargetDataHandle& Data)
{
	
	if (Data.Num() == 0 || !Data.Get(0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	OutActors = Data.Get(0)->GetActors();

	for (TWeakObjectPtr<AActor> TarActor : OutActors)
	{
		if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(TarActor.Get()))
		{
			ApplyGameplayEffectToTarget(StunStartEffect, Villan->GetAbilitySystemComponent(), GetOwnerASC());
		}
	}

	// Only the server applies the actual gameplay effects and delay
	if (HasAuthority(&CurrentActivationInfo))
	{
		

		// Start delay task to apply the real stun effect
		StunTaskWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, StunDelay);
		StunTaskWaitDelay->OnFinish.AddDynamic(this, &UZL_GA_Seven_Stun::OnFinish);
		StunTaskWaitDelay->ReadyForActivation();
	}	
}

void UZL_GA_Seven_Stun::OnFinish()
{
	// Only the server applies gameplay effects
	if (!HasAuthority(&CurrentActivationInfo))
		return;

	for (TWeakObjectPtr<AActor> TarActor : OutActors)
	{
		if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(TarActor.Get()))
		{
			ApplyGameplayEffectToTarget(StunEffect, Villan->GetAbilitySystemComponent(), GetOwnerASC());
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	OutActors.Empty();
}



