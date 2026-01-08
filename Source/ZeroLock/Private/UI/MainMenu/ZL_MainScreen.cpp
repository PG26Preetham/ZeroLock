//Copyright Preetham Mukundan (C) 2026


#include "UI/MainMenu/ZL_MainScreen.h"

#include "CommonButtonBase.h"
#include "Kismet/GameplayStatics.h"

void UZL_MainScreen::PlayBtnClicked()
{
	if (OnPlayClick.IsBound())
	{
		OnPlayClick.Broadcast();
	}
}

void UZL_MainScreen::SettingsBtnClicked()
{
	if (OnSettingsClick.IsBound())
	{
		OnSettingsClick.Broadcast();
	}
}

void UZL_MainScreen::QuitBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,true);
}

void UZL_MainScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BTN_Play->OnClicked().AddUObject(this,&ThisClass::PlayBtnClicked);
	BTN_Settings->OnClicked().AddUObject(this,&ThisClass::SettingsBtnClicked);
	BTN_Quit->OnClicked().AddUObject(this,&ThisClass::QuitBtnClicked);
}
