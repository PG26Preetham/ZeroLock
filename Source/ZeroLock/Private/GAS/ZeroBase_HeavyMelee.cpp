// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ZeroBase_HeavyMelee.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/Tasks/GAST_MeleeMoveTo.h"
#include "ZeroLock/ZeroLockCharacter.h"
// Helper Macros
#if 1
float MacroDuration = 2.f;
#define ZLOG(x) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.f, FColor::Yellow, x);
#define ZPOINT(x, c) DrawDebugPoint(GetWorld(), x, 10, c, !MacroDuration, MacroDuration);
#define ZLINE(x1, x2, c) DrawDebugLine(GetWorld(), x1, x2, c, !MacroDuration, MacroDuration);
#define ZCAPSULE(x, c) DrawDebugCapsule(GetWorld(), x, CapHH(), CapR(), FQuat::Identity, c, !MacroDuration, MacroDuration);
#else
#define ZLOG(x)
#define ZPOINT(x, c)
#define ZLINE(x1, x2, c)
#define ZCAPSULE(x, c)
#endif
class AZeroLockCharacter;

UZeroBase_HeavyMelee::UZeroBase_HeavyMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UZeroBase_HeavyMelee::MeleeDistanceFinished()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	FCollisionShape Cap = FCollisionShape::MakeSphere(100);
	FVector TraceLocation = Hero->GetActorLocation() + Hero->GetActorForwardVector()*100;
	FVector TraceEndLocation =TraceLocation + Hero->GetActorForwardVector()*200;
	TArray<FHitResult> Hits;
	FQuat RotationQuat = Hero->GetActorRotation().Quaternion();
	TArray<AZeroLockCharacter*> HitActors;
	
	if(GetWorld()->SweepMultiByObjectType(Hits,TraceLocation,TraceEndLocation,RotationQuat,ECC_Pawn,Cap,Hero->GetIgnoreCharacterParams()))
	{
		ZLOG("Found Hits");
		FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Melee.Parry"),false);
		for(int i=0;i<Hits.Num();i++)
		{
			AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Hits[i].GetActor());
			if (!Villan)
			{
				continue;
			}
			if (HitActors.Contains(Villan))
			{
				continue;
			}
			HitActors.Add(Villan);
			if (Villan->GetAbilitySystemComponent()->HasMatchingGameplayTag(ParryTag))
			{
				ZLOG("Found Parry");
				if (AbilitySystemComp && ParryEffect)
				{
					FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
					EffectContext.AddSourceObject(this);


					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(ParryEffect, 1, EffectContext);

					if (SpecHandle.IsValid())
					{
						//AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
						FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
				break;
			}
			if (AbilitySystemComp && HeavyMeleeDamageEffect)
			{
				ZLOG("Found Damage");
				FGameplayEffectContextHandle EffectContext =AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);


				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(HeavyMeleeDamageEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					
					FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),Villan->GetAbilitySystemComponent());
				}
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void UZeroBase_HeavyMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
	
	MeleeMoveTask = UGAST_MeleeMoveTo::MeleeToLocation(this,FName("HeavyMelee"),MeleeTime,MeleeSpeed);
	MeleeMoveTask->OnMeleeMoveFinished.AddDynamic(this,&UZeroBase_HeavyMelee::MeleeDistanceFinished);
	MeleeMoveTask->ReadyForActivation();
}
