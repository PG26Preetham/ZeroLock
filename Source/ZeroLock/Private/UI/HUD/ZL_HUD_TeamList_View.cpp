// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_TeamList_View.h"

#include "Zero_BasePlayerState.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Gamemode/Zero_BaseGameState.h"
#include "UI/HUD/ZL_PlayerInfoBox.h"

void UZL_HUD_TeamList_View::SetupAllyTeam(AZero_BasePlayerState* ps)
{
	TeamID = ps->TeamID;
	SetupDelegates();
}

void UZL_HUD_TeamList_View::SetupEnemyTeam(AZero_BasePlayerState* ps)
{
	if (ps->TeamID == ETeamID::TeamRed)
	{
		TeamID =ETeamID::TeamBlue;
	}
	else if (ps->TeamID == ETeamID::TeamBlue)
	{
		TeamID = ETeamID::TeamRed;
	}
	SetupDelegates();
}

void UZL_HUD_TeamList_View::AddPlayerToBox(AZero_BasePlayerState* PS)
{
	if (PS)
	{
		UZL_PlayerInfoBox* newIcon = CreateWidget<UZL_PlayerInfoBox>(this,PlayerIconClass);
		if (newIcon)
		{
			//newIcon->SetDefaultsWithPS(PS);
			if (UHorizontalBoxSlot* newSlot = HorizontalBox->AddChildToHorizontalBox(newIcon))
			{
				newSlot->SetPadding(FMargin(5.0f));
			}
		}
	}
}

void UZL_HUD_TeamList_View::SetupDelegates()
{
	AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>();
	if (!GS) return;
	if (!PlayerIconClass)return;
	TArray<AZero_BasePlayerState*> TeamPlayer;
	if (TeamID == ETeamID::TeamRed)
	{
		TeamPlayer = GS->TeamRedArray;
	}
	else if (TeamID == ETeamID::TeamBlue)
	{
		TeamPlayer = GS->TeamBlueArray;
	}
	for (AZero_BasePlayerState* ps : TeamPlayer)
	{
		AddPlayerToBox(ps);
	}
	if (TeamID == ETeamID::TeamRed)
	{
		GS->OnRedTeamArrayUpdated.AddUniqueDynamic(this,&UZL_HUD_TeamList_View::AddPlayerToBox);
	}
	else if (TeamID == ETeamID::TeamBlue)
	{
		GS->OnBlueTeamArrayUpdated.AddUniqueDynamic(this,&UZL_HUD_TeamList_View::AddPlayerToBox);
	}
}
