// Copyright Preetham Mukundan (C) 2026


#include "Xayah/ZL_Xayah_Bladecaller.h"

#include "AbilitySystemComponent.h"
#include "Weapon/Zero_BaseProjectile.h"
#include "Xayah/ZL_Xayah_CleanCut.h"
#include "Xayah/ZL_Xayah_projectile.h"

void UZL_Xayah_Bladecaller::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (!ASC) return;


		UZL_Xayah_CleanCut* TrackerInstance = nullptr;
	
		TArray<FGameplayAbilitySpec> Abilities = ASC->GetActivatableAbilities();
		TArray<FGameplayAbilitySpecHandle> AbilitiesHandles; 
		FGameplayTagContainer myTags;
		myTags.AddTag(FGameplayTag::RequestGameplayTag("Zerolock.Xayah.ClearCut",false));
		ASC->FindAllAbilitiesWithTags(AbilitiesHandles,myTags,true);
	
		for (const FGameplayAbilitySpecHandle& SpecHandle : AbilitiesHandles)
		{
		
			FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(SpecHandle);
			if (Spec)
			{
				ZLOG(Spec->Ability->GetClass()->GetName());
				TrackerInstance = Cast<UZL_Xayah_CleanCut>(Spec->GetPrimaryInstance());
				break; 
			}
		}
	
		if (TrackerInstance)
		{
			for (auto& WeakFeather : TrackerInstance->ActiveFeathers)
			{
				if (WeakFeather.IsValid())
				{
					WeakFeather->StartReturn(GetAvatarActorFromActorInfo());
				}
			}
			TrackerInstance->ActiveFeathers.Empty();
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}
