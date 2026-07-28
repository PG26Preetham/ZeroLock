// Copyright Preetham Mukundan (C) 2026


#include "Apollo/ZL_Apollo_Riposte.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Apollo/ZL_Apollo_FlawlessAdvance.h"
#include "Camera/CameraComponent.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/Tasks/ZL_Task_ApplyRootMotion.h"
#include "GAS/Tasks/ZL_WaitDelay_Task.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/RootMotionSource.h"

UZL_Apollo_Riposte::UZL_Apollo_Riposte()
{
}

void UZL_Apollo_Riposte::OnPlayerHit(AActor* HitActor)
{
	
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
	}
	if (ResistanceEffect)
	{
		Hero->GetMyAbilitySystemComp()->ApplyGameplayEffect(Hero->GetMyAbilitySystemComp(), ResistanceEffect, GetAbilityLevel());
	}
	TArray<FOverlapResult> OverlapResults;
	float SphereRadius = 100.f;
	FVector SphereLocation = Hero->GetActorLocation() + (Hero->GetActorForwardVector() * 50);
	
	
	if (GetWorld()->OverlapMultiByChannel(OverlapResults,SphereLocation,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeSphere(SphereRadius),Hero->GetIgnoreCharacterParams()))
	{
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			if (AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Overlap.GetActor()))
			{
				if (Hero->IsOnSameTeam(Villan))continue;
				if (StunEffect)
				{
					Hero->GetMyAbilitySystemComp()->ApplyGameplayEffect(Villan->GetMyAbilitySystemComp(), StunEffect, GetAbilityLevel());
				}
				
			}
		}
	}
	
	DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 12, FColor::Red, false, 12.0f);
	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,false);
	
}

void UZL_Apollo_Riposte::OnFinished()
{
	OnPlayerHit(nullptr);
}

void UZL_Apollo_Riposte::TestWait()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	FVector FD = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	if (Hero)
	{
		FD = Hero->GetFollowCamera()->GetForwardVector();
	}
	UZL_Task_ApplyRootMotion* RootMotionTask = UZL_Task_ApplyRootMotion::ApplyRootMotionConstantForceWithHit(this,FName("Dash"),FD,10000,0.1f,true,ERootMotionFinishVelocityMode::SetVelocity,FVector::ZeroVector,0.0f,false);
	RootMotionTask->OnFinish.AddDynamic(this,&UZL_Apollo_Riposte::OnFinished);
	RootMotionTask->OnPlayerHit.AddDynamic(this,&UZL_Apollo_Riposte::OnPlayerHit);
	RootMotionTask->ReadyForActivation();
}

void UZL_Apollo_Riposte::OnEventRecived(FGameplayEventData Payload)
{
	
	if (AnimMontageTask && AnimMontageTask->IsActive())
	{
		GetAbilitySystemComponentFromActorInfo()->CurrentMontageStop(-1);
		AnimMontageTask->EndTask();
	}
	if (WaitTimeTask && WaitTimeTask->IsActive())
	{
		WaitTimeTask->OnFinished.RemoveDynamic(this, &UZL_Apollo_Riposte::K2_EndAbility);
		WaitTimeTask->EndTask();
	}

	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	FVector FD = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	if (Hero)
	{
		FD = Hero->GetFollowCamera()->GetForwardVector();
	}
	UZL_Task_ApplyRootMotion* RootMotionTask = UZL_Task_ApplyRootMotion::ApplyRootMotionConstantForceWithHit(this,FName("Dash"),FD,10000,0.1f,true,ERootMotionFinishVelocityMode::SetVelocity,FVector::ZeroVector,0.0f,false);
	RootMotionTask->OnFinish.AddDynamic(this,&UZL_Apollo_Riposte::OnFinished);
	RootMotionTask->OnPlayerHit.AddDynamic(this,&UZL_Apollo_Riposte::OnPlayerHit);
	RootMotionTask->ReadyForActivation();
	
}

void UZL_Apollo_Riposte::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero && ParryEffect)
	{
		Hero->GetMyAbilitySystemComp()->ApplyGameplayEffect(Hero->GetMyAbilitySystemComp(),ParryEffect,GetAbilityLevel());
	}
	if (ParryAnimMontage)
	{
		AnimMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("AnimMontageAndWait"),ParryAnimMontage);
		AnimMontageTask->ReadyForActivation();
	}
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	WaitTimeTask = UZL_WaitDelay_Task::WaitDealyWithProgressBar(this,MaxTime);
	WaitTimeTask->OnProgress.AddDynamic(this,&UZL_Apollo_Riposte::UpdateProgressionTimer);
	WaitTimeTask->OnStarted.AddDynamic(this,&UZL_Apollo_Riposte::StartProgressionTimer);
	WaitTimeTask->OnEnd.AddDynamic(this,&UZL_Apollo_Riposte::StopProgressionTimer);
	WaitTimeTask->OnFinished.AddDynamic(this,&UZL_Apollo_Riposte::K2_EndAbility);
	WaitTimeTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,FGameplayTag::RequestGameplayTag("Event.UntouchableTrigger",false));
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UZL_Apollo_Riposte::OnEventRecived);
	WaitGameplayEvent->ReadyForActivation();
	
	
	
}

void UZL_Apollo_Riposte::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CommitAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo());
	
	if (AnimMontageTask && AnimMontageTask->IsActive())
	{
		GetAbilitySystemComponentFromActorInfo()->CurrentMontageStop(-1);
		AnimMontageTask->EndTask();
	}
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	FGameplayTag UntouchableTag = FGameplayTag::RequestGameplayTag(FName("Zerolock.Untouchable"));
	
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(UntouchableTag);

	GetAbilitySystemComponentFromActorInfo()->RemoveActiveEffectsWithGrantedTags(TagContainer);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
