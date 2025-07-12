// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZeroBase_HeavyMelee.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Camera/CameraComponent.h"
#include "GAS/Tasks/GAST_MeleeMoveTo.h"
#include "GAS/Tasks/GAST_PlayMontageAndWaitForEvent.h"
#include "ZeroLock/ZeroLockCharacter.h"

class AZeroLockCharacter;

UZeroBase_HeavyMelee::UZeroBase_HeavyMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UZeroBase_HeavyMelee::OnFinish()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UZeroBase_HeavyMelee::MeleeDistanceFinished()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	MeleeDamageExec(HeavyMeleeDamageEffect,Hero->HeavyMeleeMontage);
}

void UZeroBase_HeavyMelee::WaitOver()
{
	if (InputRTask->IsActive())
	{
		InputRTask->EndTask();
	}
	StartHeavyMelee();
}

void UZeroBase_HeavyMelee::OnInputRelased(float TimeHeld)
{
	if (TaskWaitDelay->IsActive())
	{
		TaskWaitDelay->EndTask();
	}
	if (TimeHeld >= MeleeMinHoldTime)
	{
		StartHeavyMelee();
	}
	else
	{
		AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
		MeleeDamageExec(LightMeleeDamageEffect,Hero->LightMeleeMontage);
	}
}


void UZeroBase_HeavyMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	TaskWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this,MeleeMinHoldTime);
	TaskWaitDelay->OnFinish.AddDynamic(this,&UZeroBase_HeavyMelee::WaitOver);
	TaskWaitDelay->ReadyForActivation();

	InputRTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRTask->OnRelease.AddDynamic(this,&UZeroBase_HeavyMelee::OnInputRelased);
	InputRTask->ReadyForActivation();
}

void UZeroBase_HeavyMelee::MeleeDamageExec(TSubclassOf<class UGameplayEffect> MeleeDamageEffect,
	UAnimMontage* MeleeMontage)
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (MeleeMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("HeavyMelee"),MeleeMontage);
		MontageTask->OnCompleted.AddDynamic(this,&UZeroBase_HeavyMelee::OnFinish);
		MontageTask->OnCancelled.AddDynamic(this,&UZeroBase_HeavyMelee::OnFinish);
		MontageTask->ReadyForActivation();
	}
	Hero->FOVChange(120,0.25);
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	FCollisionShape Cap = FCollisionShape::MakeSphere(100);
	FVector TraceLocation = Hero->GetActorLocation() + Hero->GetActorForwardVector()*100;
	FVector TraceEndLocation =TraceLocation + Hero->GetActorForwardVector()*200;
	TArray<FHitResult> Hits;
	FQuat RotationQuat = Hero->GetActorRotation().Quaternion();
	TArray<AZeroLockCharacter*> HitActors;
	
	if(GetWorld()->SweepMultiByObjectType(Hits,TraceLocation,TraceEndLocation,RotationQuat,ECC_Pawn,Cap,Hero->GetIgnoreCharacterParams()))
	{
		ZLOG("Found Hits");
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
				ZLOG("Found Parry");
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
			if (AbilitySystemComp && MeleeDamageEffect)
			{
				//ZLOG("Found Damage");
				FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				
				FVector KnockbackDir = (Villan->GetActorLocation() - Hero->GetActorLocation()).GetSafeNormal();
				KnockbackDir.Z=0;
				Villan->LaunchCharacter(KnockbackDir * 1000,false,false);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(MeleeDamageEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					
					FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
				}
			}
		}
	}
	if (!MeleeMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UZeroBase_HeavyMelee::StartHeavyMelee()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	//UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	Hero->FOVChange(130,0.25);
	MeleeMoveTask = UGAST_MeleeMoveTo::MeleeToLocation(this,FName("HeavyMelee"),MeleeTime,MeleeSpeed);
	MeleeMoveTask->OnMeleeMoveFinished.AddDynamic(this,&UZeroBase_HeavyMelee::MeleeDistanceFinished);
	MeleeMoveTask->ReadyForActivation();
}


