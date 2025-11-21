// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUDClass.h"

#include "UI/ZL_BaseUILayout.h"
#include "UI/ZL_UI_Tags.h"

UZL_BaseUILayout* AZL_HUDClass::ReturnRootLayer()
{
	return RootLayer;
}

void AZL_HUDClass::BeginPlay()
{
	Super::BeginPlay();

	if (!BaseUILayer)
	{
		return;
	}
	if (GetOwningPlayerController())
	{
		if (!GetOwningPlayerController()->IsLocalController()) return;
		RootLayer = Cast<UZL_BaseUILayout>(CreateWidget(GetOwningPlayerController(),BaseUILayer));
		if (RootLayer)
		{
			RootLayer->AddToViewport();
		}
	}

	PushHUD();
}

void AZL_HUDClass::PushHUD()
{
	if (IsValid(RootLayer) && PlayerInGameHUD)
	{
		RootLayer->PushWidgetToLayer(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_GAME), PlayerInGameHUD);
	}
}
