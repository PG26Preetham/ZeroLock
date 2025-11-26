// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_GameTimer.h"

#include "CommonTextBlock.h"
#include "Gamemode/Zero_BaseGameState.h"

void UZL_HUD_GameTimer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AddDelegates();
}

void UZL_HUD_GameTimer::TimeUpdated(int GameTime)
{
	TimerTextBlock->SetText(FormatTimeText(GameTime));
}

void UZL_HUD_GameTimer::AddDelegates()
{
	if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		GS->OnGameTimeUpdated.AddUniqueDynamic(this,&ThisClass::TimeUpdated);
	}
}

FText UZL_HUD_GameTimer::FormatTimeText(int32 TotalSeconds)
{
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds));
}
