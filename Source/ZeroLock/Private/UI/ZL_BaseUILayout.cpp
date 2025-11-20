// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZL_BaseUILayout.h"

#include "UI/ZL_UI_Tags.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UZL_BaseUILayout::UZL_BaseUILayout()
{
	bAutoActivate=true;
	bSupportsActivationFocus=true;
}

TOptional<FUIInputConfig> UZL_BaseUILayout::GetDesiredInputConfig() const
{
	FUIInputConfig InputConfig
	{
		ECommonInputMode::All,
		EMouseCaptureMode::CapturePermanently
	};
	return InputConfig;
}

void UZL_BaseUILayout::PushWidgetToLayer(FUITag LayerTag, TSubclassOf<UCommonActivatableWidget> Widget)
{
	if (Layer.Find(LayerTag))
	{
		TObjectPtr<UCommonActivatableWidgetContainerBase> LayerToPush = Layer.FindRef(LayerTag);
		
		if (LayerToPush)
		{
			TObjectPtr<UCommonActivatableWidget> CurrentWidget = LayerToPush->GetActiveWidget();
			if (!CurrentWidget)
			{
				LayerToPush->AddWidget<UCommonActivatableWidget>(Widget);
				GetActivatedWidgetInLayer(LayerTag);
				return;
			}

			if (!CurrentWidget.IsA(Widget))
			{
				LayerToPush->AddWidget<UCommonActivatableWidget>(Widget);
				GetActivatedWidgetInLayer(LayerTag);
				return;
			}
		}
		
	};
}

UCommonActivatableWidget* UZL_BaseUILayout::GetActivatedWidgetInLayer(FUITag LayerTag)
{
	if (Layer.Find(LayerTag))
	{
		TObjectPtr<UCommonActivatableWidgetContainerBase> LayerToSearch = Layer.FindRef(LayerTag);
		
		if (IsValid(LayerToSearch))
		{
			UCommonActivatableWidget* CurrentWidget = LayerToSearch->GetActiveWidget();
			return CurrentWidget;
		}
	}
	return nullptr;
}

void UZL_BaseUILayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	Layer.Add(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_GAME),GameStack);
	Layer.Add(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_MENU),MenuStack);
	Layer.Add(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_POPUP),PopUpStack);
	Layer.Add(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_ITEMSHOP),ItemShopStack);
	
}

void UZL_BaseUILayout::RegisterLayer(FUITag LayerTag, TObjectPtr<UCommonActivatableWidgetContainerBase> LayerStack)
{
	Layer.Add(LayerTag,LayerStack);
}
