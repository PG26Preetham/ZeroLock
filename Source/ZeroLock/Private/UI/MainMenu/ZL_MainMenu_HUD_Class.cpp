//Copyright Preetham Mukundan (C) 2026


#include "UI/MainMenu/ZL_MainMenu_HUD_Class.h"

#include "UI/ZL_BaseUILayout.h"
#include "UI/ZL_UI_Tags.h"

void AZL_MainMenu_HUD_Class::PushHUD()
{
	if (IsValid(RootLayer) && PlayerInGameHUD)
	{
		
		RootLayer->PushWidgetToLayer(FUITag::TryConvert(ZerolockUIGameplayTags::TAG_UI_LAYER_MENU), PlayerInGameHUD);

		if (APlayerController* PC = GetOwningPlayerController())
		{
			PC->SetShowMouseCursor(true);
			//PC-> SetInputMode(FInputModeUIOnly());
		}
	}
}
