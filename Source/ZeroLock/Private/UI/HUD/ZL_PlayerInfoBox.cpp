// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_PlayerInfoBox.h"

#include <string>

#include "CommonTextBlock.h"
#include "MVVMSubsystem.h"
#include "Zero_BasePlayerState.h"
#include "Components/Image.h"
#include "UI/MVVM/GameStats/ZL_VM_PlayerInfo.h"
#include "View/MVVMView.h"


void UZL_PlayerInfoBox::SetViewModel(UZL_VM_PlayerInfo* InViewModel)
{
	myVM = InViewModel;
	if (!myVM) return;
	if (UMVVMSubsystem* MVVMSubsystem = GEngine->GetEngineSubsystem<UMVVMSubsystem>())
	{

		if (UMVVMView* WidgetView = MVVMSubsystem->GetViewFromUserWidget(this))
		{					

			if (bool bSuccess = WidgetView->SetViewModel(FName("ZL_VM_PlayerInfo"), myVM))
			{
				PlayerIcon->SetBrushFromTexture(myVM->GetIcon());
				PlayerName->SetText(myVM->GetPlayerName());
				Kill->SetText(FText::FromString(FString::FromInt(myVM->GetKills())));
				Death->SetText(FText::FromString(FString::FromInt(myVM->GetDeaths())));
				Assist->SetText(FText::FromString(FString::FromInt(myVM->GetAssists())));
				
			}
		}
	}
}
