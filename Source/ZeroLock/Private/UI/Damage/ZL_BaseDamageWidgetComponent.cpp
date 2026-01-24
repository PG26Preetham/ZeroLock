// Copyright Preetham Mukundan (C) 2026


#include "UI/Damage/ZL_BaseDamageWidgetComponent.h"

#include "GameplayTagContainer.h"
#include "UI/Damage/ZL_DamagePopUpBase.h"

UZL_BaseDamageWidgetComponent::UZL_BaseDamageWidgetComponent()
{
	WidgetClass = MyWidgetClass;
}

void UZL_BaseDamageWidgetComponent::ShowDamageNumber(float Damage, FGameplayTagContainer DamageNumberTags)
{
	if (UZL_DamagePopUpBase* myWidget = Cast<UZL_DamagePopUpBase>(GetWidget()))
	{
		if (DamageNumberTags.HasTagExact(FGameplayTag::RequestGameplayTag("Damage.Tag.Spirit")))
		{
			myWidget->AddDamageNumber(Damage, 0);
		}
		else if (DamageNumberTags.HasTagExact(FGameplayTag::RequestGameplayTag("Damage.Tag.Weapon")))
		{
			myWidget->AddDamageNumber(Damage, 1);
		}
		else
		{
			myWidget->AddDamageNumber(Damage, 2);
		}
	}
}
