// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Zero_BaseWeaponAbility.h"

#include "Weapon/Zero_BaseProjectile.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZero_BaseWeaponAbility::UZero_BaseWeaponAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FGameplayTag ReloadTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Reload"),false);
	ActivationBlockedTags.AddTag(ReloadTag);
}

void UZero_BaseWeaponAbility::Fire()
{
	
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Authority)
	{
		if (Hero)
		{
			if (ProjectileClass)
			{
				FVector Location = Hero->GetActorLocation() + Hero->GetActorForwardVector().GetSafeNormal()* 10.0f;
				FRotator Rotation = Hero->GetActorRotation();
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.Owner = Hero;
				SpawnParameters.Instigator = Hero->GetInstigator();
				SpawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Fire");
				GetWorld()->SpawnActor<AZero_BaseProjectile>(ProjectileClass,Location,Rotation,SpawnParameters);
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}

void UZero_BaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	Fire();
}



