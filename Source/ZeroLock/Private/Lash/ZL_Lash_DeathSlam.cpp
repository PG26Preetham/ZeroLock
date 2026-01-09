// Copyright Preetham Mukundan (C) 2026


#include "Lash/ZL_Lash_DeathSlam.h"

#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Lash/ZL_GATargetActor_CylinderCharge.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Lash_DeathSlam::UZL_Lash_DeathSlam()
{
}

void UZL_Lash_DeathSlam::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// 1. Setup Spawn Transform at Player location
	FVector StartLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetLocation(StartLocation);

	// 2. Spawn our custom Cylinder Target Actor
	// Ensure 'Targetclass' is set to AZL_GATargetActor_CylinderCharge in the Blueprint
	AZL_GATargetActor_CylinderCharge* TargetActor = GetWorld()->SpawnActor<AZL_GATargetActor_CylinderCharge>(Targetclass, SpawnTransform);

	if (TargetActor)
	{
		TargetActor->OwningAbility = this;
		TargetActor->PrimaryPC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
		TargetActor->SetOwner(GetAvatarActorFromActorInfo());

		// Inject custom Cylinder parameters
		TargetActor->RequiredTime = LockOnThreshold;
		TargetActor->CylinderRadius = CylinderRadius;
		TargetActor->CylinderHeight = CylinderLength;
		TargetActor->ReticleClass = ReticleClassX;

		// Configure the Global Filter
		FGameplayTargetDataFilter* Filter = new FGameplayTargetDataFilter();
		Filter->RequiredActorClass = AZeroLockCharacter::StaticClass();
		Filter->SelfFilter = ETargetDataFilterSelf::TDFS_NoSelf;
		TargetActor->Filter.Filter = TSharedPtr<FGameplayTargetDataFilter>(Filter);

		TargetActor->StartTargeting(this);


		if (TargetConfirmationStyle == EGASTargetConfirmationStyle::Instant)
		{
			WaitTargetTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, FName(TEXT("CylinderTargeting")),	EGameplayTargetingConfirmation::Instant,TargetActor);
		}
		else
		{
			WaitTargetTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, FName(TEXT("CylinderTargeting")), EGameplayTargetingConfirmation::UserConfirmed, TargetActor);
		}
		
		WaitTargetTask->ValidData.AddDynamic(this, &UZL_Lash_DeathSlam::TargetConfirmed);
		WaitTargetTask->Cancelled.AddDynamic(this, &UZL_Lash_DeathSlam::TargetCancelled);
		WaitTargetTask->ReadyForActivation();

		// 4. Handle "Quick" style (Release key to confirm)
		if (TargetConfirmationStyle == EGASTargetConfirmationStyle::Quick)
		{
			UAbilityTask_WaitInputRelease* InputRTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			InputRTask->OnRelease.AddDynamic(this, &UZL_Lash_DeathSlam::TargetInputRelease);
			InputRTask->ReadyForActivation();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UZL_Lash_DeathSlam::AbilityConfirmedAction(const FGameplayAbilityTargetDataHandle& Data)
{
	if (Data.Num() == 0 || !Data.Get(0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	OutActors = Data.Get(0)->GetActors();

	ZLOG(FString::Printf(TEXT("Ability Confirmed! Locked on %d targets."), OutActors.Num()));

	if (LockOnEffect)
	{
		for (TWeakObjectPtr<AActor> Target : OutActors)
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(LockOnEffect);
			if (SpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, Data);
			}
		}
	}

	// Always call Super to handle EndAbility and cleanup
	Super::AbilityConfirmedAction(Data);
}
