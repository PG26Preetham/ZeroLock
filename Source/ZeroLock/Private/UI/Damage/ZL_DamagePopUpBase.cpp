// Copyright Preetham Mukundan (C) 2026


#include "UI/Damage/ZL_DamagePopUpBase.h"

#include "CommonTextBlock.h"

void UZL_DamagePopUpBase::AddDamageNumber(float DamageAmount, int32 TypeID)
{
	if (DamageAmount <= 0 ) return;
	
	GetWorld()->GetTimerManager().ClearTimer(DamageAddTimer);
	
	DamageTotalAmount += DamageAmount;

	MainDamageText->SetText(FText::AsNumber(DamageTotalAmount));
	MainDamageText->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(DamageAddTimer,this,&ThisClass::ClearTimer,1.0f);

	AddindividualDamageNumber(DamageAmount, TypeID);
	
}

void UZL_DamagePopUpBase::ClearTimer()
{
	DamageTotalAmount = 0;
	MainDamageText->SetText(FText::AsNumber(DamageTotalAmount));
	MainDamageText->SetVisibility(ESlateVisibility::Hidden);
}
