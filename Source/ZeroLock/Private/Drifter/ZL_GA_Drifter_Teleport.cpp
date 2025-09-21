// Fill out your copyright notice in the Description page of Project Settings.


#include "Drifter/ZL_GA_Drifter_Teleport.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "ZeroLock/ZeroLockCharacter.h"




void UZL_GA_Drifter_Teleport::OnEventRecived(FGameplayEventData Payload)
{
	//Super::OnEventRecived(Payload);
	Villan = Cast<AZeroLockCharacter>(Payload.Target);
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(Villan)) return;
	if (!IsValid(Hero)) return;

	UAbilitySystemComponent* TargetASC = Villan->GetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = Hero->GetAbilitySystemComponent();

	if (!TargetASC || !SourceASC)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	//TargetASC->AddGameplayCue(MarkedCueTag);

	ZLOG("EnemyHitRecieved");
	
	InputPTask = UAbilityTask_WaitInputPress::WaitInputPress(this,false);
	InputPTask->OnPress.AddDynamic(this, &ThisClass::OnInputPressed);
	InputPTask->ReadyForActivation();

	WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this,3);
	WaitDelay->OnFinish.AddDynamic(this, &ThisClass::OnTimeFinish);
	WaitDelay->ReadyForActivation();
	
}

void UZL_GA_Drifter_Teleport::OnInputPressed(float TimeWaited)
{
	WaitDelay->ExternalCancel();
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	

	if (Villan && IsValid(Hero))
	{
		Hero->SetActorLocation(Villan->GetActorLocation()+ FVector(0,0,200));
		//Villan->GetAbilitySystemComponent()->RemoveGameplayCue(MarkedCueTag);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UZL_GA_Drifter_Teleport::OnTimeFinish()
{
	if (Villan && IsValid(Villan))
	{
		//Villan->GetAbilitySystemComponent()->RemoveGameplayCue(MarkedCueTag);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
