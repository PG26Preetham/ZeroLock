//Copyright Preetham Mukundan (C) 2026


#include "UI/MainMenu/ZL_MainMenu.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UI/MainMenu/ZL_LoginPage.h"
#include "UI/MainMenu/ZL_MainScreen.h"

void UZL_MainMenu::ShowLobbyScreen()
{
}

void UZL_MainMenu::ShowSettingsScreen()
{
}

void UZL_MainMenu::LoginDone()
{
	MainMenuSetup();
}

void UZL_MainMenu::LoginSetup()
{
	if (LoginPage)
	{
		MainSwitcher->SetActiveWidget(LoginPage);
		LoginPage->OnLoginDone.AddDynamic(this,&ThisClass::LoginDone);
	}
}

void UZL_MainMenu::MainMenuSetup()
{
	if (MainPage)
	{
		MainSwitcher->SetActiveWidget(MainPage);
		MainPage->OnPlayClick.AddUniqueDynamic(this,&ThisClass::ShowLobbyScreen);
		MainPage->OnSettingsClick.AddUniqueDynamic(this,&ThisClass::ShowSettingsScreen);
	}
}

void UZL_MainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LoginSetup();
}
