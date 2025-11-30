// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZL_Base_Passive_ApplyToSelf.h"

#include "AbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_Base_Passive_ApplyToSelf::OnEventRecived(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEventRecieved called for %s"), *GetName());
	const AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Payload.Target);
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Villan)) return;
	if (!IsValid(Hero)) return;

	//UAbilitySystemComponent* TargetASC = Villan->GetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = Hero->GetAbilitySystemComponent();

	if (!SourceASC) return;

	if (PassiveEffectToApply)
	{
		//ZLOG("Found Damage");
		FGameplayEffectContextHandle EffectContext =SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
				
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(PassiveEffectToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			//ZLOG("UBasePassive_GameplayAbility::OnEventRecived");
			FActiveGameplayEffectHandle GEHandle = SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

}
