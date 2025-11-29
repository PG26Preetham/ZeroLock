// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/ZL_LoginPage.h"
#include"CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Gamemode/Zero_BaseGameInstance.h"

void UZL_LoginPage::LoginResultRecieved(bool bSuccess)
{
	if (bSuccess)
	{
		OnLoginDone.Broadcast();
	}
	else
	{
		LoginResultText->SetText(FText::FromString("Login Failed!"));
		LoginResultText->SetVisibility(ESlateVisibility::Visible);
		LoginBtn->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZL_LoginPage::LoginClicked()
{
	LoginBtn->SetVisibility(ESlateVisibility::Collapsed);
	UZero_BaseGameInstance* myGM = Cast<UZero_BaseGameInstance>(GetGameInstance());
	if (myGM)
	{
		myGM->OnLoginResult.AddUniqueDynamic(this,&ThisClass::LoginResultRecieved);
		myGM->Login();
	}
}

void UZL_LoginPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (LoginResultText)
	{
		LoginResultText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (LoginBtn)
	{
		LoginBtn->OnClicked().AddUObject(this,&ThisClass::LoginClicked);
	}
}
