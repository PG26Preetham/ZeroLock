// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZeroBase_LightMelee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ZeroLock/ZeroLockCharacter.h"



UZeroBase_LightMelee::UZeroBase_LightMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UZeroBase_LightMelee::OnFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UZeroBase_LightMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	float animplayLength =0.3f;
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero->LightMeleeMontage)
	{
		//Hero->PlayAnimMontage(LightMeleeMontage);
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("LightMelee"),Hero->LightMeleeMontage);
		MontageTask->OnCompleted.AddDynamic(this,&UZeroBase_LightMelee::OnFinish);
		MontageTask->OnCancelled.AddDynamic(this,&UZeroBase_LightMelee::OnFinish);
		MontageTask->ReadyForActivation();
		animplayLength = Hero->LightMeleeMontage->GetPlayLength();
	}
	UAbilityTask_WaitDelay* TaskWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this,animplayLength);
	TaskWaitDelay->OnFinish.AddDynamic(this,&UZeroBase_LightMelee::OnFinish);
	TaskWaitDelay->ReadyForActivation();
	
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	FCollisionShape Cap = FCollisionShape::MakeSphere(100);
	FVector TraceLocation = Hero->GetActorLocation() + Hero->GetActorForwardVector()*100;
	FVector TraceEndLocation =TraceLocation + Hero->GetActorForwardVector()*200;
	TArray<FHitResult> Hits;
	FQuat RotationQuat = Hero->GetActorRotation().Quaternion();
	TArray<AZeroLockCharacter*> HitActors;
	
	if(GetWorld()->SweepMultiByObjectType(Hits,TraceLocation,TraceEndLocation,RotationQuat,ECC_Pawn,Cap,Hero->GetIgnoreCharacterParams()))
	{
		
		FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Melee.Parry"),false);
		for(int i=0;i<Hits.Num();i++)
		{
			AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Hits[i].GetActor());
			if (!Villan)
			{
				continue;
			}
			if (HitActors.Contains(Villan))
			{
				continue;
			}
			HitActors.Add(Villan);
			if (Villan->GetAbilitySystemComponent()->HasMatchingGameplayTag(ParryTag))
			{
				if (AbilitySystemComp && ParryEffect)
				{
					FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
					EffectContext.AddSourceObject(this);


					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(ParryEffect, 1, EffectContext);

					if (SpecHandle.IsValid())
					{
						//AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
						FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
				break;
			}
			if (AbilitySystemComp && LightMeleeDamageEffect)
			{
				FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);


				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(LightMeleeDamageEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					
					FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
				}
			}
		}
	}
	if (!Hero->LightMeleeMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	

}
