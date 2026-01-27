//Copyright Preetham Mukundan (C) 2026


#include "Zero_BasePlayerState.h"

#include "Gamemode/Zero_BaseGameState.h"
#include "Net/UnrealNetwork.h"
#include "UI/MVVM/ZL_VM_Attributes.h"
#include "UI/MVVM/GameStats/ZL_VM_PlayerInfo.h"
#include "ZeroLock/ZeroLock.h"

AZero_BasePlayerState::AZero_BasePlayerState()
{
	Kills = 0;
	Assists = 0;
	Deaths = 0;
	TeamID = ETeamID::TeamNull;

	bReplicates = true; // Make sure this actor replicates
}


void AZero_BasePlayerState::SetTeamID(ETeamID id_team)
{
	if (HasAuthority())
	{
		TeamID = id_team;
		//ZLOG_COLOR_TIME("SERVER_Set_TEAM",FColor::Blue,10);
		if (OnTeamChanged.IsBound())
		{
			OnTeamChanged.Broadcast(TeamID);
		}
		if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
		{
			GS->RefreshTeamLists();
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
		if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
		{
			GS->RefreshTeamLists();
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
		if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
		{
			GS->RefreshTeamLists();
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

void AZero_BasePlayerState::SetPlayerIconImage(UTexture2D* NewImage)
{
	if (HasAuthority())
	{
		
	}
}


void AZero_BasePlayerState::SetCurrentVM(UZL_VM_Attributes* InVM)
{
	CurrentVM = InVM;
	if (PlayerIconVM)
	{
		PlayerIconVM->SetVM_Attributes(InVM);
	}
}

void AZero_BasePlayerState::OnRep_Kills()
{
	if (OnKillsChanged.IsBound())
	{
		//ZLOG_COLOR_TIME("CLIENT_KILL_ADDED",FColor::Red,20);
		OnKillsChanged.Broadcast(Kills);
	}
	if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		GS->UpdatePlayerInVM(this);
	}
}

void AZero_BasePlayerState::OnRep_Assists()
{
	if (OnAssistsChanged.IsBound())
	{
		OnAssistsChanged.Broadcast(Assists);
	}
	if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		GS->UpdatePlayerInVM(this);
	}
}

void AZero_BasePlayerState::OnRep_Deaths()
{
	if (OnDeathsChanged.IsBound())
	{
		//ZLOG_COLOR_TIME("CLIENT_DEATH_ADDED",FColor::Red,20);
		OnDeathsChanged.Broadcast(Deaths);
	}
	if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		GS->UpdatePlayerInVM(this);
	}
}

void AZero_BasePlayerState::OnRep_TeamID()
{
	if (OnTeamChanged.IsBound())
	{
	//	ZLOG_COLOR_TIME("CLIENT_Set_TEAM",FColor::Red,10);
		OnTeamChanged.Broadcast(TeamID);
	}
	if (AZero_BaseGameState* GS = GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		GS->RefreshTeamLists();
	}
}

void AZero_BasePlayerState::OnRep_PlayerImage()
{
	if (OnPlayerIconChanged.IsBound())
	{
		OnPlayerIconChanged.Broadcast(PlayerImage);
	}
}

void AZero_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZero_BasePlayerState, Kills);
	DOREPLIFETIME(AZero_BasePlayerState, Assists);
	DOREPLIFETIME(AZero_BasePlayerState, Deaths);
	DOREPLIFETIME(AZero_BasePlayerState, TeamID);
	DOREPLIFETIME(AZero_BasePlayerState, PlayerImage);
}
