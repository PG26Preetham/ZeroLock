// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BasePassive_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ZeroLock/ZeroLockCharacter.h"



UBasePassive_GameplayAbility::UBasePassive_GameplayAbility()
{
	
	TargetStyle = EGASTargetConfirmationStyle::Passive;
	NetExecutionPolicy =EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UBasePassive_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
}

void UBasePassive_GameplayAbility::OnEventRecived(const FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEventRecieved called for %s"), *GetName());
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

void UBasePassive_GameplayAbility::OnHeroEventRecived(const FGameplayEventData& GameplayEventData)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnEventRecieved called for %s"), *GetName());
	OnEventRecived(GameplayEventData);
}

void UBasePassive_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData* TriggerEventData)
{
//	ZLOG("Passive effect on");
	UE_LOG(LogTemp, Warning, TEXT("ActivateAbility started for %s"), *GetName());
	/*AZeroLockCharacter* hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (hero)
	{
		hero->OnWeaponHitEventReceived.AddUniqueDynamic(this,&ThisClass::OnHeroEventRecived);
	}*/
	 UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.WeaponHit",false));
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UBasePassive_GameplayAbility::OnEventRecived);
	WaitGameplayEvent->ReadyForActivation();
}
