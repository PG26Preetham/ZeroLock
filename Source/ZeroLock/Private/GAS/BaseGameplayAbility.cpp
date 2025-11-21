// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseGameplayAbility.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLLock.Abilities"),false));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseGameplayAbility::SetSlot(EGameplayAbilitySlot slot)
{
	Slot = slot;
}

void UBaseGameplayAbility::ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> GEToApply,
                                                       UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC)
{
	if (GEToApply == nullptr || TargetASC == nullptr || SourceASC == nullptr)
	{
		ZLOG("CancelledInBaseGameplayAbility::ApplyGameplyEffectToTarget");
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (HasAuthority(&CurrentActivationInfo))
	{
		//ZLOG("Found Damage");
		FGameplayEffectContextHandle EffectContext =SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
				
	
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(GEToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			ZLOG("ApplyingEffect::ApplyGameplyEffectToTarget");
			FActiveGameplayEffectHandle GEHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
		}
	}
}

void UBaseGameplayAbility::ApplyGameplyEffectToSelf(TSubclassOf<UGameplayEffect> GEToApply,
	UAbilitySystemComponent* SourseASC)
{
	
	if (GEToApply == nullptr || SourseASC == nullptr)
	{
		ZLOG("CancelledInBaseGameplayAbility::ApplyGameplyEffectToSelf");
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if (HasAuthority(&CurrentActivationInfo))
	{
		FGameplayEffectContextHandle EffectContext =SourseASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);


		FGameplayEffectSpecHandle SpecHandle = SourseASC->MakeOutgoingSpec(GEToApply, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			ZLOG("BuffDone");
			FActiveGameplayEffectHandle GEHandle = SourseASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UAbilitySystemComponent* UBaseGameplayAbility::GetOwnerASC()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero )
	{
		return Hero->GetAbilitySystemComponent();
	}
	return nullptr;
}

void UBaseGameplayAbility::SetInputID(EGASAbilityInputID in)
{
	AbilityInputID=in;
}



