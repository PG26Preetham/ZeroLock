// Copyright Preetham Mukundan (C) 2026


#include "Xayah/ZL_Xayah_Featherstorm.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Xayah/ZL_Xayah_projectile.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Xayah_Featherstorm::UZL_Xayah_Featherstorm()
{
}

void UZL_Xayah_Featherstorm::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero) return;

	FVector BackwardsDir = -Hero->GetActorForwardVector();

	FVector LaunchVel = BackwardsDir;
	LaunchVel.Z = 0.25;
	LaunchVel *= LaunchStrength;
	
	Hero->LaunchCharacter(LaunchVel, true, true);

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, FireDelay);
	DelayTask->OnFinish.AddDynamic(this, &UZL_Xayah_Featherstorm::FireFeathers);
	DelayTask->ReadyForActivation();
}

void UZL_Xayah_Featherstorm::FireFeathers()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		UWorld* World = GetWorld();
		if (!AvatarActor || !World) return;

		FVector ActorLoc = AvatarActor->GetActorLocation();
		FVector Forward = AvatarActor->GetActorForwardVector();
		FVector AimDir = Forward.RotateAngleAxis(DownwardAngle, AvatarActor->GetActorRightVector());
		float AngleStep = (ProjectileCount > 1) ? (ConeAngle / (ProjectileCount - 1)) : 0.0f;
		float CurrentAngle = -ConeAngle / 2.0f;
		for (int32 i = 0; i < ProjectileCount; i++)
		{
			FVector ProjDir = AimDir.RotateAngleAxis(CurrentAngle, FVector::UpVector);        
			FTransform SpawnTransform(ProjDir.Rotation(), ActorLoc + (ProjDir * 100.0f));
			AZL_Xayah_projectile* Proj = World->SpawnActorDeferred<AZL_Xayah_projectile>(ProjectileClass, SpawnTransform, AvatarActor, Cast<APawn>(AvatarActor));
			if (Proj)
			{
				Proj->SetAutoPull(true);
				Proj->FinishSpawning(SpawnTransform);
			}
			CurrentAngle += AngleStep;
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	}
}
