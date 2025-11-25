// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Zero_BaseGameState.h"

#include "Net/UnrealNetwork.h"

void AZero_BaseGameState::AddKill(int32 TeamID)
{
	if (TeamID == 0) TeamRedScore++;
	else TeamBlueScore++;
}

void AZero_BaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZero_BaseGameState, TeamRedScore);
	DOREPLIFETIME(AZero_BaseGameState, TeamBlueScore);
	DOREPLIFETIME(AZero_BaseGameState, TeamRedArray);
	DOREPLIFETIME(AZero_BaseGameState, TeamBlueArray);
}
