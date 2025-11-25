// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseComp/ZL_Stat_Box.h"

#include "CommonTextBlock.h"
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
