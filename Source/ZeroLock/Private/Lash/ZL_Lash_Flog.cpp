// Copyright Preetham Mukundan (C) 2026


#include "Lash/ZL_Lash_Flog.h"

#include "Camera/CameraComponent.h"
#include "Engine/OverlapResult.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Lash_Flog::UZL_Lash_Flog()
{
}

void UZL_Lash_Flog::OnAnimationPointTrigger()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
	}
	CommitAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo());
	TArray<FOverlapResult> OverlapResults;
	FVector Origin = Hero->GetPawnViewLocation();
	FVector Direction = Hero->GetFollowCamera()->GetForwardVector();
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	TArray<AZeroLockCharacter*> OutVillans;
	ActorsToIgnore.Add(Hero);
	//CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	if (ConeTraceMulti(GetWorld(),Origin,Direction.Rotation(),ConeHeight,ConeAngle/2, UEngineTypes::ConvertToTraceType(ECC_Pawn),true,ActorsToIgnore,EDrawDebugTrace::ForDuration,HitResults,OutVillans,true))
	{
		if (GetCurrentActorInfo()->IsNetAuthority())
		{
			if (OutVillans.Num() > 0)
			{
				for (AZeroLockCharacter* villan : OutVillans)
				{
					if (!villan) continue;
					if (Hero->IsOnSameTeam(villan)) continue;
				
					Hero->GetMyAbilitySystemComp()->ApplySpiritDamage(villan->GetMyAbilitySystemComp(),FlogDamage);
					Hero->GetMyAbilitySystemComp()->ApplyHeal(Hero->GetMyAbilitySystemComp(),FlogHeal);
					
				}
			}
		}
		
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UZL_Lash_Flog::OnAnimationCompleted()
{
	//Super::OnAnimationCompleted();
	OnAnimationPointTrigger();
}
