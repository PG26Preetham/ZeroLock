// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZL_BasePlayAnimation_AndDo.h"

#include "AbilitySystemComponent.h"
#include "GAS/Tasks/GAST_PlayMontageAndWaitForEvent.h"

UZL_BasePlayAnimation_AndDo::UZL_BasePlayAnimation_AndDo()
{
}

void UZL_BasePlayAnimation_AndDo::OnCanelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnAnimationCancelled();
}

void UZL_BasePlayAnimation_AndDo::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnAnimationCancelled();
}


void UZL_BasePlayAnimation_AndDo::OnReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	OnAnimationPointTrigger();
}

void UZL_BasePlayAnimation_AndDo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	if (!EventTagToWait.IsValid() || !MontageToPlay)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
	}
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	PlayMontageTask = UGAST_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this,FName("MontagetoPlay"),MontageToPlay,EventTagToWait);
	PlayMontageTask->OnCancelled.AddDynamic(this,&UZL_BasePlayAnimation_AndDo::OnCanelled);
	PlayMontageTask->OnCompleted.AddDynamic(this,&UZL_BasePlayAnimation_AndDo::OnCompleted);
	PlayMontageTask->EventReceived.AddDynamic(this,&UZL_BasePlayAnimation_AndDo::OnReceived);
	PlayMontageTask->ReadyForActivation();
}

void UZL_BasePlayAnimation_AndDo::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UZL_BasePlayAnimation_AndDo::OnAnimationPointTrigger()
{
	ZLOG("event");
	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true, false);
}

void UZL_BasePlayAnimation_AndDo::OnAnimationCancelled()
{
	ZLOG("Cancelled");
	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true, true);
}
