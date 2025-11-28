// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_Abilities_Bar.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ZL_HUD_AbilityIcon.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommonTextBlock.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/ZL_GameplayTags.h"

void UZL_Abilities_Bar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitMap();
	AddDelegates();
	if (!Hero) return;
	if (Hero->HasAuthority())
	{
		GrantIconToAbilitiesX();
	}
	
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




void UZL_Abilities_Bar::GrantIconToAbilityWithTag(const UBaseGameplayAbility* AbilitytoAdd, FGameplayTag TagToAddTO,
	FGameplayAbilitySpec* InSpec, FGameplayAbilitySpecHandle InSpecHandle)
{
	if (!AbilitytoAdd) return;
	FString AbilityID=FString::FromInt(static_cast<int32>(AbilitytoAdd->AbilityInputID));

	UZL_HUD_AbilityIcon* Icon =AbilityTagMap.FindRef(TagToAddTO);
	AbilityTagMap.Remove(TagToAddTO);
	if (AbilityTagMap.IsEmpty())
	{
		Hero->GetMyAbilitySystemComp()->OnNewAbilityAdded.RemoveDynamic(this,&ThisClass::UZL_Abilities_Bar::NewAbilityAdded);
	}
	if (Icon)
	{
		Icon->Setup(AbilitytoAdd,InSpec,InSpecHandle);
		//Icon->SetupTag(TagToAddTO);
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
					GrantIconToAbilityWithTag(Ability, Tag,Spec,AbilitySpecHandle);
					break;
				}
			}
		}
		
	}
	AbilitiesList->SetText(FText::FromString(HeroName));
}

void UZL_Abilities_Bar::NewAbilityAdded(FGameplayAbilitySpec& AbilitySpec)
{
	//GrantIconToAbilitiesX();
	FGameplayAbilitySpec* Spec = &AbilitySpec;
	FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySpec.Handle;
	const UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(Spec->Ability);
	if (Spec && Ability)
	{
		
		for (const FGameplayTag& Tag : Spec->GetDynamicSpecSourceTags())
		{
			if (AbilityTagMap.Contains(Tag))
			{
				GrantIconToAbilityWithTag(Ability, Tag,Spec,AbilitySpecHandle);
				break;
			}
		}
	}
}

void UZL_Abilities_Bar::AddDelegates()
{
	Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		Hero->GetMyAbilitySystemComp()->OnNewAbilityAdded.AddUniqueDynamic(this,&ThisClass::NewAbilityAdded);
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
