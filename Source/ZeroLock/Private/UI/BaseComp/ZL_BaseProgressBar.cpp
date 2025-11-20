// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseComp/ZL_BaseProgressBar.h"

#include "Components/Image.h"
#include "CommonTextBlock.h"

void UZL_BaseProgressBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitDynamicMaterialInstance();
}

void UZL_BaseProgressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AddToDelegate();
}

void UZL_BaseProgressBar::AddToDelegate()
{
}

void UZL_BaseProgressBar::InitDynamicMaterialInstance()
{
	if (!ParentMaterial) return;
	
	DynamicMI = UMaterialInstanceDynamic::Create(ParentMaterial, this, DynamicMaterialName);
	DynamicMI->SetVectorParameterValue("BGColor", BackgroundColor);
	DynamicMI->SetVectorParameterValue("ProgressColorEnd", ProgressColorEnd);
	DynamicMI->SetVectorParameterValue("ProgressColorStart", ProgressColorStart);
	DynamicMI->SetScalarParameterValue("BackgroundOpacity", BackgroundOpacity);
	DynamicMI->SetScalarParameterValue("BarOpacity", BarOpacity);
	DynamicMI->SetScalarParameterValue("CornerRoundness", CornerRoundness);
	DynamicMI->SetScalarParameterValue("GlowMax", GlowMax);
	DynamicMI->SetScalarParameterValue("GlowOpacity", GlowOpacity);
	DynamicMI->SetScalarParameterValue("GlowSize", GlowSize);
	DynamicMI->SetScalarParameterValue("ShapeOpacity", ShapeOpacity);
	DynamicMI->SetScalarParameterValue("PipSizeX", PipSizeX);
	DynamicMI->SetScalarParameterValue("PipSizeY", PipSizeY);
	
	if (StartLeft)
	{
		DynamicMI->SetScalarParameterValue("StartLeft", 0.0f);
	}
	else
	{
		DynamicMI->SetScalarParameterValue("StartLeft", 1.0f);
	}
	WidgetImage->SetBrushFromMaterial(DynamicMI);
}

void UZL_BaseProgressBar::OnBarValueChanged(float NewValue, float MaxValue)
{
	DynamicMI->SetScalarParameterValue("FillAmount", NewValue/MaxValue);
	FString HealthText = FString::FromInt((int)NewValue);
	FString MaxHealthText = "/" + FString::FromInt((int)MaxValue);
	MaxHealthDisplayText->SetText(FText::FromString(MaxHealthText));
	HealthDisplayText->SetText(FText::FromString(HealthText));
}
