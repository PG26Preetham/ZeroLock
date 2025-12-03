// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseComp/ZL_Stat_Box.h"

#include "AbilitySystemComponent.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "ZeroLock/ZeroLock.h"

void UZL_Stat_Box::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StatValueText->SetText(FText::FromString("0"));
	
}

void UZL_Stat_Box::NewValueChanged(int32 newValue)
{
	FString newValueString = FString::FromInt(newValue);
	StatValueText->SetText(FText::FromString(newValueString));
}

void UZL_Stat_Box::NewValueChangedFloat(float newValue)
{
	FString newValueString = FString::Printf(TEXT("%.1f"), newValue);
	StatValueText->SetText(FText::FromString(newValueString));
}

void UZL_Stat_Box::SetIcon(UTexture2D* Icon)
{
	StatIcon->SetBrushFromTexture(Icon);
}

void UZL_Stat_Box::AtrributeChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	float newValue= OnAttributeChangeData.NewValue;
	NewValueChangedFloat(newValue);
}

void UZL_Stat_Box::InitGameplayAttribute(FGameplayAttribute AttributeToBind, UAbilitySystemComponent* FromASC,
	UAttributeSet* FromAS)
{
	if (FromASC && FromAS && AttributeToBind.IsValid())
	{
		StatName->SetText(FText::FromString(AttributeToBind.GetName()));
		float BaseValue = AttributeToBind.GetNumericValue(FromAS);
		NewValueChangedFloat(BaseValue);
		FromASC->GetGameplayAttributeValueChangeDelegate(AttributeToBind).AddUObject(this,&UZL_Stat_Box::AtrributeChanged);
	}
}




