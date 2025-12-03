// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseGameplayAbility.h"

#include "GameplayTagsManager.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "GAS/ZL_GameplayTags.h"
#include "GAS/ZL_GE_BaseCooldown.h"
#include "ZeroLock/ZeroLockCharacter.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities"),false));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ZeroLLock.Abilities"),false));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CooldownGameplayEffectClass = UZL_GE_BaseCooldown::StaticClass();
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

void UBaseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(  "Ability.Cooldown.Duration" )), GetCoolDownTime());
		FActiveGameplayEffectHandle GEHandle=ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		if (GEHandle.IsValid())
		{
			ZLOG("CustomSetByCaller");
		}
	}
}

float UBaseGameplayAbility::GetCoolDownTime() const
{

	float cooldownTime = CooldownDuration.GetValueAtLevel(GetAbilityLevel());
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero)
	{
		float CDR = Hero->GetMyAttributeSet()->GetCooldownReduction();
		return (cooldownTime*(1-CDR/100));
	}
	return cooldownTime;
}

const FGameplayTagContainer* UBaseGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}




