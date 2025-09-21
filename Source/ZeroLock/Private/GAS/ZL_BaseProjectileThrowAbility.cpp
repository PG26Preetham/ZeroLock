// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZL_BaseProjectileThrowAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Zero_BaseProjectile.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_BaseProjectileThrowAbility::FireProjectile()
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
			AZero_BaseProjectile* proj =GetWorld()->SpawnActor<AZero_BaseProjectile>(ProjectileClass,Location,Rotation,SpawnParameters);
			if (proj)
			{
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Fire");
				proj->SetOwner(Hero);
				proj->OwnerCharacter = Hero;
			}
		}
	}
	
}

void UZL_BaseProjectileThrowAbility::OnEventRecived(FGameplayEventData Payload)
{
	ZLOG("EventRecieved");
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UZL_BaseProjectileThrowAbility::MissOnEventRecived(FGameplayEventData Payload)
{
	ZLOG("EventMiss");
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UZL_BaseProjectileThrowAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	FireProjectile();
	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,ProjectileHitTag);
	WaitGameplayEvent->EventReceived.AddDynamic(this,&UZL_BaseProjectileThrowAbility::OnEventRecived);
	WaitGameplayEvent->ReadyForActivation();

	MissWaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,ProjectileMissTag);
	MissWaitGameplayEvent->EventReceived.AddDynamic(this,&UZL_BaseProjectileThrowAbility::MissOnEventRecived);
	MissWaitGameplayEvent->ReadyForActivation();
}
