// Fill out your copyright notice in the Description page of Project Settings.


#include "Zero_BasePlayerState.h"

#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLock.h"

AZero_BasePlayerState::AZero_BasePlayerState()
{
	Kills = 0;
	Assists = 0;
	Deaths = 0;
	TeamID = 0;

	bReplicates = true; // Make sure this actor replicates
}

void AZero_BasePlayerState::SetTeamID(int32 NewTeamID)
{
	if (HasAuthority())
	{
		TeamID = NewTeamID;
			//ZLOG_COLOR_TIME("SERVER_Set_TEAM",FColor::Blue,10);
		if (OnTeamChanged.IsBound())
		{
			OnTeamChanged.Broadcast(TeamID);
		}
	}
}

void AZero_BasePlayerState::AddKill()
{
	
	if (HasAuthority())
	{
		Kills++;
		if (OnKillsChanged.IsBound())
		{
			//ZLOG_COLOR_TIME("SERVER_KILL_ADDED",FColor::Blue,20);
			OnKillsChanged.Broadcast(Kills);
		}
	}
}

void AZero_BasePlayerState::AddDeath()
{
	if (HasAuthority())
	{
		Deaths++;
		if (OnDeathsChanged.IsBound())
		{
		//	ZLOG_COLOR_TIME("SERVER_Death_ADDED",FColor::Blue,20);
			OnDeathsChanged.Broadcast(Deaths);
		}
	}
}

void AZero_BasePlayerState::AddAssist()
{
	if (HasAuthority())
	{
		Assists++;
		if (OnAssistsChanged.IsBound())
		{
			OnAssistsChanged.Broadcast(Assists);
		}
	}
}

void AZero_BasePlayerState::OnRep_Kills()
{
	if (OnKillsChanged.IsBound())
	{
		//ZLOG_COLOR_TIME("CLIENT_KILL_ADDED",FColor::Red,20);
		OnKillsChanged.Broadcast(Kills);
	}
	
}

void AZero_BasePlayerState::OnRep_Assists()
{
	if (OnAssistsChanged.IsBound())
	{
		OnAssistsChanged.Broadcast(Assists);
	}
}

void AZero_BasePlayerState::OnRep_Deaths()
{
	if (OnDeathsChanged.IsBound())
	{
		//ZLOG_COLOR_TIME("CLIENT_DEATH_ADDED",FColor::Red,20);
		OnDeathsChanged.Broadcast(Deaths);
	}
}

void AZero_BasePlayerState::OnRep_TeamID()
{
	if (OnTeamChanged.IsBound())
	{
	//	ZLOG_COLOR_TIME("CLIENT_Set_TEAM",FColor::Red,10);
		OnTeamChanged.Broadcast(TeamID);
	}
}

void AZero_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZero_BasePlayerState, Kills);
	DOREPLIFETIME(AZero_BasePlayerState, Assists);
	DOREPLIFETIME(AZero_BasePlayerState, Deaths);
	DOREPLIFETIME(AZero_BasePlayerState, TeamID);
}
