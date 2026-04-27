// Copyright Preetham Mukundan (C) 2026


#include "UI/Settings/ZL_MainSettingsMenu.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "UI/Settings/ZL_BTN_SubMenu.h"
#include "UI/Settings/ZL_SettingsSubMenu.h"

void UZL_MainSettingsMenu::GenerateTabsFromMap(const TMap<FName, USubMenuManagerViewModel*>& ViewModelMap)
{
	if (!SubMenuBtnClass || !SubMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing sub classes for btn or widget"));
		return;
	}
	SubMenuBTNContainer->ClearChildren();
	SubMenuSwitcher->ClearChildren();
	for (const TPair<FName, USubMenuManagerViewModel*>& Pair : ViewModelMap)
	{
		FName TabName = Pair.Key;
		USubMenuManagerViewModel* ChildVM = Pair.Value;

		UZL_SettingsSubMenu* NewContentWidget = CreateWidget<UZL_SettingsSubMenu>(this, SubMenuWidgetClass);
		if (NewContentWidget)
		{
			NewContentWidget->SetupVM(ChildVM);
			SubMenuSwitcher->AddChild(NewContentWidget);
		}
		
		UZL_BTN_SubMenu* NewTabButton = CreateWidget<UZL_BTN_SubMenu>(this, SubMenuBtnClass);
		if (NewTabButton)
		{
			NewTabButton->InitializeTab(TabName, NewContentWidget, SubMenuSwitcher);
			SubMenuBTNContainer->AddChild(NewTabButton);
		}
	}
}
