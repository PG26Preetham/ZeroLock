// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_Abilities_Bar.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ZL_HUD_AbilityIcon.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"

void UZL_Abilities_Bar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitMap();
	
}

void UZL_Abilities_Bar::NativeOnActivated()
{
	Super::NativeOnActivated();

	//GrantIconToAbilitiesX();
}


void UZL_Abilities_Bar::NativePreConstruct()
{
	Super::NativePreConstruct();
	AddDelegates();
}

void UZL_Abilities_Bar::GrantIconToAbilities(const UBaseGameplayAbility* AbilitytoAdd, EGASAbilityInputID SlotToAddIn)
{
	if (!AbilitytoAdd) return;
	UZL_HUD_AbilityIcon* Icon =AbilityIconMap.FindRef(SlotToAddIn);
	if (Icon)
	{
		Icon->Setup(AbilitytoAdd);
	}
}

void UZL_Abilities_Bar::GrantIconToAbilitiesX()
{
	if (!Hero) return;
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	Hero->GetAbilitySystemComponent()->GetAllAbilities(AbilitySpecHandles);
	for (FGameplayAbilitySpecHandle AbilitySpecHandle : AbilitySpecHandles)
	{
		bool bIsInstanced;
		const UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(Hero->GetAbilitySystemComponent(),AbilitySpecHandle,bIsInstanced));
		
		if (Ability)
		{
			GrantIconToAbilities(Ability,Ability->AbilityInputID);
		}
		
	}
}


void UZL_Abilities_Bar::AddDelegates()
{
	Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		
		//Hero->AddAbilityIconDelegate.AddUniqueDynamic(this,&ThisClass::GrantIconToAbilities);

		
		
		GrantIconToAbilitiesX();		
	}
	
}

void UZL_Abilities_Bar::InitMap()
{
	AbilityIconMap.Add(EGASAbilityInputID::Secondry_Attack,Ability1);
	AbilityIconMap.Add(EGASAbilityInputID::Ability_1,Ability2);
	AbilityIconMap.Add(EGASAbilityInputID::Ability_2,Ability3);
	AbilityIconMap.Add(EGASAbilityInputID::Ultimate,Ultimate);
	AbilityIconMap.Add(EGASAbilityInputID::Ultimate,Ultimate);
}
