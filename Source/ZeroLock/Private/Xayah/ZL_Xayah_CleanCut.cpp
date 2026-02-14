// Copyright Preetham Mukundan (C) 2026


#include "Xayah/ZL_Xayah_CleanCut.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Xayah/ZL_Xayah_projectile.h"

UZL_Xayah_CleanCut::UZL_Xayah_CleanCut()
{
	TargetStyle = EGASTargetConfirmationStyle::Passive;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UZL_Xayah_CleanCut::OnEventRecived(FGameplayEventData Payload)
{
	const AZL_Xayah_projectile* ConstFeather = Cast<AZL_Xayah_projectile>(Payload.OptionalObject);
    
	if (ConstFeather)
	{
		ZLOG("feather down");
		AZL_Xayah_projectile* MutableFeather = const_cast<AZL_Xayah_projectile*>(ConstFeather);
		ActiveFeathers.Add(TWeakObjectPtr<AZL_Xayah_projectile>(MutableFeather));
	}
}

void UZL_Xayah_CleanCut::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
}

void UZL_Xayah_CleanCut::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateAbility started for %s"), *GetName());

	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.Xayah.FeatherDown",false));
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UZL_Xayah_CleanCut::OnEventRecived);
	WaitGameplayEvent->ReadyForActivation();
}

