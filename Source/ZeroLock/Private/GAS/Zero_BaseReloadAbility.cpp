//Copyright Preetham Mukundan (C) 2026


#include "GAS/Zero_BaseReloadAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZero_BaseReloadAbility::UZero_BaseReloadAbility()
{
}

void UZero_BaseReloadAbility::OnFinish()
{
	
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	if (AbilitySystemComp && ReloadEffect)
	{
		FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);


		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(ReloadEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UZero_BaseReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	TaskWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this,ReloadTime);
	TaskWaitDelay->OnFinish.AddDynamic(this,&UZero_BaseReloadAbility::OnFinish);
	TaskWaitDelay->ReadyForActivation();
}

void UZero_BaseReloadAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
