// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_AbilityIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLockCharacter.h"


void UZL_HUD_AbilityIcon::Setup(const UBaseGameplayAbility* abilityToBindTo, FGameplayAbilitySpec* InSpec,FGameplayAbilitySpecHandle SpecHandle)
{
	if (abilityToBindTo == nullptr) return;

	Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	AbilityToBindTo = abilityToBindTo;
	AbilitySpec = InSpec;
	AbilitySpecHandleToStore = SpecHandle;
	AbilityIcon->SetBrushFromTexture(AbilityToBindTo->IconImage);
	AbilityName->SetText(FText::FromString(AbilityToBindTo->AbilityName));
	AbilityDescription->SetText(FText::FromString(AbilityToBindTo->AbilityDescription));
	if (Hero)
	{
		AddToDelegate();
	}
}

void UZL_HUD_AbilityIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AbilityDescription->SetVisibility(ESlateVisibility::Collapsed);
	AbilityName->SetVisibility(ESlateVisibility::Collapsed);
}

void UZL_HUD_AbilityIcon::AbilityActivated(UGameplayAbility* GameplayAbility)
{
	if (GameplayAbility == nullptr) return;
	if (GameplayAbility->GetCurrentAbilitySpecHandle() != AbilitySpecHandleToStore)return ;
	const FGameplayTagContainer* Tags = GameplayAbility->GetCooldownTags();
	if (Tags->IsValid() && !alreadyRegstered)
	{
		
		alreadyRegstered = true;
		for (FGameplayTag Tag : *Tags)
		{
			if (Tag.IsValid())
			{
				Hero->GetMyAbilitySystemComp()->RegisterGameplayTagEvent(Tag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UZL_HUD_AbilityIcon::OnCooldownTagChanged);
			}
		}
	}
	AbilityActivatedEvent();
}

void UZL_HUD_AbilityIcon::OnCooldownTagChanged(FGameplayTag GameplayTag, int NewCount)
{
	FGameplayAbilitySpec* Spec = Hero->GetMyAbilitySystemComp()->FindAbilitySpecFromHandle(AbilitySpecHandleToStore);
	if (!Spec) return;
	const FGameplayTagContainer* AbilityCooldownTags=	Spec->Ability->GetCooldownTags();
	if (!AbilityCooldownTags->HasTag(GameplayTag)) return;

	if (NewCount > 0)
	{
		ZLOG("CoolDownStart");
		
		// Update UI here
	}
	else
	{
		ZLOG("CoolDownEnded");
		AbilityIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZL_HUD_AbilityIcon::AddToDelegate()
{
	if (AbilityToBindTo == nullptr) return;
	if (AbilitySpec == nullptr) return;
	if (AbilityToBindTo->TargetStyle == EGASTargetConfirmationStyle::Passive) return;
	if (!Hero) return;
	Hero->GetMyAbilitySystemComp()->AbilityActivatedCallbacks.AddUObject(this,&ThisClass::AbilityActivated);
}

void UZL_HUD_AbilityIcon::AbilityActivatedEvent()
{
	//TODO Add animation and handle change to UI on ability Activated

	AbilityIcon->SetVisibility(ESlateVisibility::Collapsed);
}
