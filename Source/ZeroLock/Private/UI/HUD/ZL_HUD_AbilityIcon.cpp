// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_AbilityIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "GAS/BaseGameplayAbility.h"


void UZL_HUD_AbilityIcon::Setup(const UBaseGameplayAbility* abilityToBindTo)
{
	
	if (abilityToBindTo == nullptr) return;

	AbilityToBindTo = abilityToBindTo;
	
	AbilityIcon->SetBrushFromTexture(AbilityToBindTo->IconImage);
	AbilityName->SetText(FText::FromString(AbilityToBindTo->AbilityName));
	AbilityDescription->SetText(FText::FromString(AbilityToBindTo->AbilityDescription));
	AddToDelegate();
}

void UZL_HUD_AbilityIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AbilityDescription->SetVisibility(ESlateVisibility::Collapsed);
	AbilityName->SetVisibility(ESlateVisibility::Collapsed);
}

void UZL_HUD_AbilityIcon::AddToDelegate()
{
	
}
