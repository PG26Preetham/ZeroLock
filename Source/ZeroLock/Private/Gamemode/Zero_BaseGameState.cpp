// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Zero_BaseGameState.h"

#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLock.h"

void AZero_BaseGameState::AddKill(ETeamID TeamID)
{
	if (TeamID == ETeamID::TeamRed) TeamRedScore++;
	else if (TeamID == ETeamID::TeamBlue) TeamBlueScore++;
}

void AZero_BaseGameState::AddPSToTeamArray(AZero_BasePlayerState* psToAdd, ETeamID teamToAddto)
{
	if (HasAuthority())
	{
		if (teamToAddto == ETeamID::TeamRed)
		{
			TeamRedArray.Add(psToAdd);
			if (OnRedTeamArrayUpdated.IsBound())
			{
				ZLOG_COLOR_TIME("SERVER_TEAM_RED_ASSIGN",FColor::Red,30);
				OnRedTeamArrayUpdated.Broadcast(psToAdd);
			}
			
		}
		else if (teamToAddto == ETeamID::TeamBlue)
		{
				ZLOG_COLOR_TIME("SERVER_TEAM_RED_ASSIGN",FColor::Blue,30);
			TeamBlueArray.Add(psToAdd);
			if (OnBlueTeamArrayUpdated.IsBound())
			{
				OnBlueTeamArrayUpdated.Broadcast(psToAdd);
			}
		}
	}
}

void AZero_BaseGameState::IncrementGameTime()
{
	if (HasAuthority())
	{
		myGameTime++;
		
		if (OnGameTimeUpdated.IsBound())
		{
			OnGameTimeUpdated.Broadcast(myGameTime);
		}
	}
}

void AZero_BaseGameState::On_BlueTeamArray()
{
	if (TeamBlueArray.IsEmpty()) return;
	if (OnBlueTeamArrayUpdated.IsBound())
	{
		OnBlueTeamArrayUpdated.Broadcast(TeamBlueArray.Last());
	}
}

void AZero_BaseGameState::On_RedTeamArray()
{
	if (TeamRedArray.IsEmpty()) return;
	if (OnRedTeamArrayUpdated.IsBound())
	{
		OnRedTeamArrayUpdated.Broadcast(TeamRedArray.Last());
	}
}

void AZero_BaseGameState::On_RepGameTime()
{
	if (OnGameTimeUpdated.IsBound())
	{
		OnGameTimeUpdated.Broadcast(myGameTime);
	}
}

void AZero_BaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZero_BaseGameState, TeamRedScore);
	DOREPLIFETIME(AZero_BaseGameState, TeamBlueScore);
	DOREPLIFETIME(AZero_BaseGameState, TeamRedArray);
	DOREPLIFETIME(AZero_BaseGameState, TeamBlueArray);
	DOREPLIFETIME(AZero_BaseGameState, myGameTime);
}
