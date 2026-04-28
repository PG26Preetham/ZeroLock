// Copyright Preetham Mukundan (C) 2026


#include "UI/Settings/ZL_MainSettingsMenu.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "CommonWidgetCarousel.h"
#include "CommonWidgetCarouselNavBar.h"
#include "Components/ScrollBox.h"
#include "UI/Settings/ZL_BTN_SubMenu.h"
#include "UI/Settings/ZL_NamedCarouselNavBar.h"
#include "UI/Settings/ZL_SettingsSubMenu.h"

void UZL_MainSettingsMenu::GenerateTabsFromMap(const TMap<FName, USubMenuManagerViewModel*>& ViewModelMap)
{
	if (!SubMenuBtnClass || !SubMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing sub classes for btn or widget"));
		return;
	}
	SubMenuCarousel->ClearChildren();
	for (const TPair<FName, USubMenuManagerViewModel*>& Pair : ViewModelMap)
	{
		FName TabName = Pair.Key;
		USubMenuManagerViewModel* ChildVM = Pair.Value;
		
		//SubMenuNavBar->SetLinkedCarousel(SubMenuCarousel);
		UZL_SettingsSubMenu* NewContentWidget = CreateWidget<UZL_SettingsSubMenu>(this, SubMenuWidgetClass);
		if (NewContentWidget)
		{
			NewContentWidget->SubMenuName = TabName;
			NewContentWidget->SetupVM(ChildVM);
			SubMenuCarousel->AddChild(NewContentWidget);
		}
		
	}
	
	SubMenuNavBar->SetLinkedCarousel(SubMenuCarousel);
	
}
