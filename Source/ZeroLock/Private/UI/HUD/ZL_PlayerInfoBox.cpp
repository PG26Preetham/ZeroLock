// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_PlayerInfoBox.h"

#include "CommonTextBlock.h"
#include "Zero_BasePlayerState.h"
#include "Components/Image.h"

void UZL_PlayerInfoBox::KillUpdate(int NewStatValue)
{
	Kill->SetText(FText::FromString(FString::FromInt(NewStatValue)));
}

void UZL_PlayerInfoBox::DeathUpdate(int NewStatValue)
{
	Death->SetText(FText::FromString(FString::FromInt(NewStatValue)));
}

void UZL_PlayerInfoBox::AssistUpdate(int NewStatValue)
{
	Assist->SetText(FText::FromString(FString::FromInt(NewStatValue)));
}

void UZL_PlayerInfoBox::SetDefaultsWithPS(AZero_BasePlayerState* PS)
{
	if (!PS)return;

	ZLOG("BindPlayerIcon");

	PlayerIcon->SetBrushFromTexture(PS->PlayerImage);
	PlayerName->SetText(FText::FromString("Player Name"));
	Kill->SetText(FText::FromString(FString::FromInt(PS->Kills)));
	Death->SetText(FText::FromString(FString::FromInt(PS->Deaths)));
	Assist->SetText(FText::FromString(FString::FromInt(PS->Assists)));

	PS->OnKillsChanged.AddUniqueDynamic(this,&ThisClass::KillUpdate);
	PS->OnDeathsChanged.AddUniqueDynamic(this,&ThisClass::DeathUpdate);
	PS->OnAssistsChanged.AddUniqueDynamic(this,&ThisClass::AssistUpdate);
}
