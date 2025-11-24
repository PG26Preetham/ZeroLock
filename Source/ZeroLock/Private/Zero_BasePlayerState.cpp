// Fill out your copyright notice in the Description page of Project Settings.


#include "Zero_BasePlayerState.h"

#include "Net/UnrealNetwork.h"

AZero_BasePlayerState::AZero_BasePlayerState()
{
}

void AZero_BasePlayerState::AddKill()
{
	Kills++; 
}

void AZero_BasePlayerState::AddDeath()
{
	Deaths++;
}

void AZero_BasePlayerState::AddAssist()
{
	Assists++;
}

void AZero_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZero_BasePlayerState, TeamID);
	DOREPLIFETIME(AZero_BasePlayerState, Kills);
	DOREPLIFETIME(AZero_BasePlayerState, Deaths);
	DOREPLIFETIME(AZero_BasePlayerState, Assists);
}
