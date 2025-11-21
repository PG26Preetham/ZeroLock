// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_Abilities_Bar.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ZL_HUD_AbilityIcon.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommonTextBlock.h"
#include "GAS/ZL_GameplayTags.h"

void UZL_Abilities_Bar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitMap();
	AddDelegates();
	GrantIconToAbilitiesX();
}

void UZL_Abilities_Bar::NativeOnActivated()
{
	Super::NativeOnActivated();

	//GrantIconToAbilitiesX();
}


void UZL_Abilities_Bar::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UZL_Abilities_Bar::GrantIconToAbilities(const UBaseGameplayAbility* AbilitytoAdd, EGASAbilityInputID SlotToAddIn)
{
	if (!AbilitytoAdd) return;
	FString AbilityID=FString::FromInt(static_cast<int32>(AbilitytoAdd->AbilityInputID));
	UZL_HUD_AbilityIcon* Icon =AbilityIconMap.FindRef(SlotToAddIn);
	if (Icon)
	{
		Icon->Setup(AbilitytoAdd);
	}
}

void UZL_Abilities_Bar::GrantIconToAbilityWithTag(const UBaseGameplayAbility* AbilitytoAdd, FGameplayTag TagToAddTO)
{
	if (!AbilitytoAdd) return;
	FString AbilityID=FString::FromInt(static_cast<int32>(AbilitytoAdd->AbilityInputID));

	UZL_HUD_AbilityIcon* Icon =AbilityTagMap.FindRef(TagToAddTO);
	if (Icon)
	{
		Icon->Setup(AbilitytoAdd);
	}
}

void UZL_Abilities_Bar::GrantIconToAbilitiesX()
{
	if (!Hero) return;
	FString HeroName = "";
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	Hero->GetAbilitySystemComponent()->GetAllAbilities(AbilitySpecHandles);
	for (FGameplayAbilitySpecHandle AbilitySpecHandle : AbilitySpecHandles)
	{
		bool bIsInstanced;
		const UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(Hero->GetAbilitySystemComponent(),AbilitySpecHandle,bIsInstanced));
		FGameplayAbilitySpec* Spec = Hero->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpecHandle);

		if (Spec)
		{
			for (const FGameplayTag& Tag : Spec->DynamicAbilityTags)
			{
				if (AbilityTagMap.Contains(Tag))
				{
					GrantIconToAbilityWithTag(Ability, Tag);
					break;
				}
			}
		}
		
	}
	AbilitiesList->SetText(FText::FromString(HeroName));
}

void UZL_Abilities_Bar::GrantIconToAbilitiesNew1()
{
	FString HeroName = "";
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	for (FMyAbilityMap abilityMap : Hero->AbilitiesArray)
	{
		UBaseGameplayAbility* Ability = abilityMap.Ability;

		if (Ability)
		{
			HeroName += "/ " + Ability->AbilityName;
			GrantIconToAbilities(Ability,abilityMap.InputID);
		}
	}
}

void UZL_Abilities_Bar::GrantIconToAbilitiesNew2()
{
	FString HeroName = "";
	const TArray<FGameplayAbilitySpec>& AbilitySpecs = Hero->GetAbilitySystemComponent()->GetActivatableAbilities();
	for (const FGameplayAbilitySpec& Spec : AbilitySpecs)
	{
		UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(Spec.GetPrimaryInstance()); // may be nullptr if InstancedPerExecution

		if (Ability)
		{
			HeroName += "/ " + Ability->AbilityName;
			GrantIconToAbilities(Ability,Ability->AbilityInputID);
		}
		
	}
}


void UZL_Abilities_Bar::GrantIconToAbilitiesY(UBaseGameplayAbility* AbilitytoAdd, EGASAbilityInputID SlotToAddIn)
{
	if (!AbilitytoAdd) return;
	UZL_HUD_AbilityIcon* Icon =AbilityIconMap.FindRef(SlotToAddIn);
	if (Icon)
	{
		Icon->Setup(AbilitytoAdd);
	}
	
}

void UZL_Abilities_Bar::AddDelegates()
{
	Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		
		//Hero->AddAbilityIconDelegate.AddUniqueDynamic(this,&ThisClass::GrantIconToAbilitiesY);
	}
	
}

void UZL_Abilities_Bar::InitMap()
{
	AbilityIconMap.Add(EGASAbilityInputID::Secondry_Attack,Ability1);
	AbilityIconMap.Add(EGASAbilityInputID::Ability_1,Ability2);
	AbilityIconMap.Add(EGASAbilityInputID::Ability_2,Ability3);
	AbilityIconMap.Add(EGASAbilityInputID::Ultimate,Ultimate);
	//AbilityIconMap.Add(EGASAbilityInputID::Ultimate,Ultimate);

	AbilityTagMap.Add(ZerolockGameplayTagsForBinding::TAG_INPUT_SECONDRY,Ability1);
	AbilityTagMap.Add(ZerolockGameplayTagsForBinding::TAG_INPUT_ABILITY_1,Ability2);
	AbilityTagMap.Add(ZerolockGameplayTagsForBinding::TAG_INPUT_ABILITY_1,Ability3);
	AbilityTagMap.Add(ZerolockGameplayTagsForBinding::TAG_INPUT_ULTIMATE,Ultimate);
}
