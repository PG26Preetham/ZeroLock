//Copyright Preetham Mukundan (C) 2026


#include "Drifter/ZL_Drifter_EternalNight.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Zero_BasePlayerState.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Engine/OverlapResult.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"



void UZL_Drifter_EternalNight::OnWeaponEventTrigger(FGameplayEventData Payload)
{
	const AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Payload.Target);
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Villan)) return;
	if (!IsValid(Hero)) return;
	Hero->GetMyAbilitySystemComp()->ApplySpiritDamage(Villan->GetMyAbilitySystemComp(),SpiritDamage);
}

void UZL_Drifter_EternalNight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthority(&ActivationInfo)) return;
	AZeroLockCharacter* Avatar = Cast<AZeroLockCharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	FVector Origin = Avatar->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxRange);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, Sphere);
	TArray<AZeroLockCharacter*> EnemyToEffect;
	for (FOverlapResult& OverlapResult : Overlaps)
	{
		if (AZeroLockCharacter* OverlapActor = Cast<AZeroLockCharacter>(OverlapResult.GetActor()))
		{
			AZero_BasePlayerState* Target_PS = OverlapActor->GetPlayerState<AZero_BasePlayerState>();
			AZero_BasePlayerState* Source_PS = Avatar->GetPlayerState<AZero_BasePlayerState>();
			if (Source_PS && Target_PS)
			{
				if (Source_PS->TeamID != Target_PS->TeamID)
				{
					EnemyToEffect.AddUnique(OverlapActor);
				}
			}
		}
	}
	CurrentActiveEffectHandles.Empty();
	if (BlindEffectClass)
	{
		FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(BlindEffectClass, 1.f, Context);
			

		for (int32 i = 0; i < FMath::Min(MaxTargets, EnemyToEffect.Num()); ++i)
		{
				UAbilitySystemComponent* TargetASC = EnemyToEffect[i]->GetAbilitySystemComponent();
				if (TargetASC)
				{
					FActiveGameplayEffectHandle ActiveHandle = Avatar->GetMyAbilitySystemComp()->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(),TargetASC);
					if (ActiveHandle.IsValid())
					{
						CurrentActiveEffectHandles.Add(ActiveHandle);
					}
				}
			
		}
	}
	CommitAbility(Handle,ActorInfo,ActivationInfo);
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
	DelayTask->OnFinish.AddDynamic(this, &UZL_Drifter_EternalNight::OnDelayFinished);
	DelayTask->ReadyForActivation();
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.WeaponHit"));
	WaitGameplayEventTask->EventReceived.AddDynamic(this,&UZL_Drifter_EternalNight::OnWeaponEventTrigger);
	WaitGameplayEventTask->ReadyForActivation();
}

void UZL_Drifter_EternalNight::OnDelayFinished()
{
	for (FActiveGameplayEffectHandle& Handle : CurrentActiveEffectHandles)
	{
		if (Handle.IsValid())
		{
			UAbilitySystemComponent* TargetASC = Handle.GetOwningAbilitySystemComponent();
			if (TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(Handle);
			}
		}
	}

	CurrentActiveEffectHandles.Empty();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
