// Copyright Preetham Mukundan (C) 2026


#include "Apollo/ZL_Apollo_DisengagingSigil.h"

#include "Camera/CameraComponent.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Apollo_DisengagingSigil::UZL_Apollo_DisengagingSigil()
{
}

void UZL_Apollo_DisengagingSigil::OnAnimationPointTrigger()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,true);
	}
	
	FVector Start = Hero->GetActorLocation();
	FVector End = Start + ( Hero->GetFollowCamera()->GetForwardVector() * 200.0f);
	FQuat Rotation = Hero->GetActorQuat();
    
	
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(EffectRadius, 100.f);
    
	DrawDebugCapsule(GetWorld(),End,50,EffectRadius,Rotation,FColor::Red,false,10,1);
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Hero);
	if (KnockBackMontage)
	{
		Hero->GetAbilitySystemComponent()->PlayMontage(this,GetCurrentActivationInfo(),KnockBackMontage,1);
	}
	
	
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults,Start,End,Rotation,ECC_Pawn,Capsule,Params);

	if (HitResults.Num()>0)
	{
		for (int i=0;i<HitResults.Num();i++)
		{
			if (AZeroLockCharacter* HitHero = Cast<AZeroLockCharacter>(HitResults[i].GetActor()))
			{
				if(!Hero->IsOnSameTeam(HitHero))
				{
					Hero->GetMyAbilitySystemComp()->ApplySpiritDamage(HitHero->GetMyAbilitySystemComp(),BaseDamageValue.GetValueAtLevel(GetAbilityLevel()));
				}
			}
		}
	}
	FVector ForwardCM = Hero->GetFollowCamera()->GetForwardVector().GetSafeNormal();
	ForwardCM.Z = -1;
	FVector LaunchVelocity  = ForwardCM *(-1 * KnockbackStrength);
	Hero->LaunchCharacter(LaunchVelocity,true,true);
	CommitAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo());
	
	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true,false);

}
