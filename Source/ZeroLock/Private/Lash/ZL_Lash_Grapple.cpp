// Copyright Preetham Mukundan (C) 2026


#include "Lash/ZL_Lash_Grapple.h"

#include "Camera/CameraComponent.h"
#include "Engine/OverlapResult.h"
#include "ZeroLock/ZeroLockCharacter.h"


UZL_Lash_Grapple::UZL_Lash_Grapple()
{
}

void UZL_Lash_Grapple::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
	}
	
	TArray<FOverlapResult> OverlapResults;
	FVector Origin = Hero->GetPawnViewLocation();
	FVector Direction = Hero->GetFollowCamera()->GetForwardVector();
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	TArray<AZeroLockCharacter*> OutVillans;
	ActorsToIgnore.Add(Hero);
	//CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	if (ConeTraceMulti(GetWorld(),Origin,Direction.Rotation(),ConeHeight,ConeAngle/2, UEngineTypes::ConvertToTraceType(ECC_Pawn),true,ActorsToIgnore,EDrawDebugTrace::None,HitResults,OutVillans,true))
	{
		if (OutVillans.Num() > 0)
		{
			for (AZeroLockCharacter* OutVillan : OutVillans)
			{
				if (OutVillan)
				{
					FVector LaunchDiraction = (OutVillan->GetActorLocation() - Hero->GetActorLocation()).GetSafeNormal();
					LaunchDiraction.Z = 0.75;
					FVector LaunchVelocity = LaunchDiraction * GrappleLaunchStrength;
					LaunchVelocity.Z+= HeightBoostInGrapple;
					Hero->LaunchCharacter(LaunchVelocity,true,true);
					CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
					EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,false);
		
				}
			}
			
		}
		else
		{
			EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
		}
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
	}
}
