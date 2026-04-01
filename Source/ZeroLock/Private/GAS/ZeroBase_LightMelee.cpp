//Copyright Preetham Mukundan (C) 2026

#include "GAS/ZeroBase_LightMelee.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Engine/OverlapResult.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZeroBase_LightMelee::UZeroBase_LightMelee()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    
    // Make the ability locally predicted so the client doesn't wait for the server to swing
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UZeroBase_LightMelee::OnFinish()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UZeroBase_LightMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
    AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
    if (!Hero)
    {
       EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
       return;
    }
    if (Hero->LightMeleeMontage)
    {
       UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("LightMelee"), Hero->LightMeleeMontage);
       MontageTask->OnCompleted.AddDynamic(this, &UZeroBase_LightMelee::OnFinish);
       MontageTask->OnInterrupted.AddDynamic(this, &UZeroBase_LightMelee::OnFinish); // Good practice to catch interruptions
       MontageTask->OnCancelled.AddDynamic(this, &UZeroBase_LightMelee::OnFinish);
       MontageTask->ReadyForActivation();
    }
    else
    {
       EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
       return;
    }

    if (Hero->HasAuthority())
    {
        UAbilitySystemComponent* AbilitySystemComp = Hero->GetAbilitySystemComponent();
       
        FCollisionShape Cap = FCollisionShape::MakeSphere(100.f);
        FVector OverlapLocation = Hero->GetActorLocation() + Hero->GetActorForwardVector() * 100.f;
        TArray<FOverlapResult> Overlaps;
        TArray<AZeroLockCharacter*> HitActors;
        
        if(GetWorld()->OverlapMultiByObjectType(Overlaps, OverlapLocation, FQuat::Identity, FCollisionObjectQueryParams(ECC_GameTraceChannel1), Cap))
        {
           FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Melee.Parry"), false);
           
           for(const FOverlapResult& Overlap : Overlaps)
           {
              AZeroLockCharacter* Villan = Cast<AZeroLockCharacter>(Overlap.GetActor());
              

              if (!Villan || Villan == Hero || HitActors.Contains(Villan))
              {
                 continue;
              }
              
              HitActors.Add(Villan);
              
              if (Villan->GetAbilitySystemComponent()->HasMatchingGameplayTag(ParryTag))
              {
                 if (AbilitySystemComp && ParryEffect)
                 {
                    FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
                    EffectContext.AddSourceObject(this);

                    FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(ParryEffect, 1, EffectContext);

                    if (SpecHandle.IsValid())
                    {
                       AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                    }
                 }
                 break; 
              }
              
              // Apply Damage
              if (AbilitySystemComp && LightMeleeDamageEffect)
              {
                 Hero->GetMyAbilitySystemComp()->ApplyMeleeDamage(Villan->GetMyAbilitySystemComp(),30);
              }
           }
        }
    }
}