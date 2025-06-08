// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Zero_BaseSelfBuff.h"

#include "AbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZero_BaseSelfBuff::UZero_BaseSelfBuff()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UZero_BaseSelfBuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	for (TSubclassOf<UGameplayEffect>& Buffs : BuffOrDebufsToApply)
	{
		if (AbilitySystemComp && Buffs)
		{
			FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
			EffectContext.AddSourceObject(this);


			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(Buffs, 1, EffectContext);

			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
	
}
