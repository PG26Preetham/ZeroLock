// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_MainSettingsMenu.generated.h"

class UZL_KeyBindingScreen;
class UInputAction;
class UScrollBox;
class UCommonActivatableWidgetSwitcher;
class USubMenuManagerViewModel;
class UZL_SettingsSubMenu;
class UZL_BTN_SubMenu;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_MainSettingsMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI Setup")
	TSubclassOf<UZL_BTN_SubMenu> SubMenuBtnClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Setup")
	TSubclassOf<UZL_SettingsSubMenu> SubMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI Setup")
	TSubclassOf<UZL_KeyBindingScreen> KeyBindingScreen;
	
	UFUNCTION(BlueprintCallable, Category = "UI Logic")
	void GenerateTabsFromMap(const TMap<FName, USubMenuManagerViewModel*>& ViewModelMap);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCommonWidgetCarousel* SubMenuCarousel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UZL_NamedCarouselNavBar* SubMenuNavBar;
};
