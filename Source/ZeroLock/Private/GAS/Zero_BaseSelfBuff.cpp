//Copyright Preetham Mukundan (C) 2026


#include "GAS/Zero_BaseSelfBuff.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "ZeroLock/ZeroLock.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZero_BaseSelfBuff::UZero_BaseSelfBuff()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UZero_BaseSelfBuff::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this,TimeBeforeBuff);
	DelayTask->OnFinish.AddDynamic(this,&UZero_BaseSelfBuff::OnFinishedDelay);
	DelayTask->ReadyForActivation();
}

void UZero_BaseSelfBuff::OnFinishedDelay()
{
	if (!HasAuthority(&CurrentActivationInfo))
	{
		// Only run the buff logic on the server
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
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
				ZLOG("BuffDone");
				FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}
