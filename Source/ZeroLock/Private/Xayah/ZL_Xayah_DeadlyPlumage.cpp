// Copyright Preetham Mukundan (C) 2026


#include "Xayah/ZL_Xayah_DeadlyPlumage.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Xayah_DeadlyPlumage::UZL_Xayah_DeadlyPlumage()
{
}

void UZL_Xayah_DeadlyPlumage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
		if (Hero)
		{
			if (BuffClass)
			{
				Hero->GetMyAbilitySystemComp()->ApplyGameplayEffect(Hero->GetMyAbilitySystemComp(),BuffClass,GetAbilityLevel());
			}
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
}

void UZL_Xayah_DeadlyPlumage::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		if (HasAuthority(&CurrentActivationInfo))
		{
			AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
			if (Hero)
			{
				Hero->GetMyAbilitySystemComp()->ApplyGameplayEffectWithStacks(Hero->GetMyAbilitySystemComp(),FeatherDownClassClass,1,3);
			}
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
