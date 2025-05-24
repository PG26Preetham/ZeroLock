// Fill out your copyright notice in the Description page of Project Settings.


#include "MonkeyBoi/Abilities/ZL_Ruyibang.h"

#include "GAS/Tasks/GAST_PlayMontageAndWaitForEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Ruyibang::UZL_Ruyibang()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Ability1"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	

	Range = 1000.0f;
	Damage = 12.0f;
	
}

void UZL_Ruyibang::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	if(!SLamAnimMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	UAnimMontage* MontageToPlay = SLamAnimMontage;

	// Play fire montage and wait for event telling us to spawn the projectile
	UGAST_PlayMontageAndWaitForEvent* Task = UGAST_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UZL_Ruyibang::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UZL_Ruyibang::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UZL_Ruyibang::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UZL_Ruyibang::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UZL_Ruyibang::EventReceived);
	// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
	Task->ReadyForActivation();
}

void UZL_Ruyibang::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UZL_Ruyibang::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UZL_Ruyibang::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Montage told us to end the ability before the montage finished playing.
	// Montage was set to continue playing animation even after ability ends so this is okay.
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Only spawn projectiles on the Server.
	// Predicting projectiles is an advanced topic not covered in this example.
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("ZeroLock.MonkeyBoi.Stomp")))
	{
		AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
		if (!Hero)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		//Hero->LaunchCharacter(FVector(0,0,1000),true,true);
		TArray<AActor*> ignoreActors;
		ignoreActors.Add(Hero);
		FVector CentreOfSphere = Hero->GetActorLocation() ;
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> outActors;
		//DrawDebugSphere(GetWorld(),GetActorLocation(),BlastRadius,32,FColor::Orange,true,5);
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(),CentreOfSphere,Range,traceObjectTypes,AZeroLockCharacter::StaticClass(),ignoreActors,outActors);
		for(int i = 0; i < outActors.Num(); i++)
		{
			if(AZeroLockCharacter* hit =  Cast<AZeroLockCharacter>(outActors[i]))
			{
				//FVector CentreOfSphere = (hit->GetActorLocation() - Hero->GetActorLocation()).GetSafeNormal();
				//hit->LaunchCharacter(CentreOfSphere * 10000,true,true);
			}
		}
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
