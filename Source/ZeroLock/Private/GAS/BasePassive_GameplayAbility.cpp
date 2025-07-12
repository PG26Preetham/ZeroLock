// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BasePassive_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ZeroLock/ZeroLockCharacter.h"



UBasePassive_GameplayAbility::UBasePassive_GameplayAbility()
{
	TargetStyle = EGASTargetConfirmationStyle::Passive;
}

void UBasePassive_GameplayAbility::OnEventRecived(FGameplayEventData Payload)
{
	
	const AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Payload.Target);
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Villan)) return;
	if (!IsValid(Hero)) return;

	UAbilitySystemComponent* TargetASC = Villan->GetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = Hero->GetAbilitySystemComponent();

	if (!TargetASC || !SourceASC) return;

	if (PassiveEffectToApply)
	{
		//ZLOG("Found Damage");
		FGameplayEffectContextHandle EffectContext =SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
				
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(PassiveEffectToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			//ZLOG("UBasePassive_GameplayAbility::OnEventRecived");
			FActiveGameplayEffectHandle GEHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
		}
	}

}

void UBasePassive_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.WeaponHit",false));
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UBasePassive_GameplayAbility::OnEventRecived);
	WaitGameplayEvent->ReadyForActivation();
}
