//Copyright Preetham Mukundan (C) 2026


#include "Weapon/Zero_BaseWeaponAbility.h"

#include "AbilitySystemComponent.h"
#include "ZeroBaseCharacterMovementComp.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Zero_BaseProjectile.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZero_BaseWeaponAbility::UZero_BaseWeaponAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FGameplayTag ReloadTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Weapon.Reloading"),false);
	ActivationBlockedTags.AddTag(ReloadTag);
}

void UZero_BaseWeaponAbility::Fire()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero)
	{
		
		if (ProjectileClass)
		{
				
			FVector Location = Hero->GetActorLocation() + Hero->GetFollowCamera()->GetForwardVector().GetSafeNormal()* 100.0f;
			FRotator Rotation = Hero->GetActorRotation();
			FVector CamTraceStartLocation = Hero->GetFollowCamera()->GetComponentLocation();
			FVector CamTraceEndLocation = CamTraceStartLocation + Hero->GetFollowCamera()->GetForwardVector() * 10000.0f;
			FHitResult Hit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Hero);
			Rotation = UKismetMathLibrary::FindLookAtRotation(Location, CamTraceEndLocation);
			
			if (GetWorld()->LineTraceSingleByChannel(Hit,CamTraceStartLocation,CamTraceEndLocation,ECC_Pawn,QueryParams))
			{
				FVector HitLoc = Hit.ImpactPoint;
				Rotation = UKismetMathLibrary::FindLookAtRotation(Location, HitLoc);
			}
				
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = Hero;
			SpawnParameters.Instigator = Hero->GetInstigator();
			SpawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Fire");
			AZero_BaseProjectile* proj =GetWorld()->SpawnActor<AZero_BaseProjectile>(ProjectileClass,Location,Rotation,SpawnParameters);
			if (proj)
			{
				proj->SetOwner(Hero);
				proj->OwnerCharacter = Hero;
			}
			if (UZeroBaseCharacterMovementComp* MC = Cast<UZeroBaseCharacterMovementComp>(Hero->GetCharacterMovement()))
			{
				if (MC->IsCustomMovementMode(ECustomMovementMode::CMOVE_Slide))
				{
					EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
					return;
				}
			}
			CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	
}

void UZero_BaseWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	if (GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Authority)
	{
		Fire();
	}

	
}



