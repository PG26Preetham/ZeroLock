// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/GameStats/ZL_VM_GameState.h"

void UZL_VM_GameState::SetFormattedGameTime(const FText& NewGameTime)
{
	UE_MVVM_SET_PROPERTY_VALUE(FormattedGameTime, NewGameTime);
}

void UZL_VM_GameState::AddAllyTeam(UZL_VM_PlayerInfo* NewAllyTeam)
{
	AllyTeam.AddUnique(NewAllyTeam);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(AllyTeam);
}

void UZL_VM_GameState::AddEnemyTeam(UZL_VM_PlayerInfo* NewEnemyTeam)
{
	EnemyTeam.AddUnique(NewEnemyTeam);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EnemyTeam);
}

void UZL_VM_GameState::EmptyAllyTeam()
{
	AllyTeam.Empty();
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(AllyTeam);
}

void UZL_VM_GameState::EmptyEnemyTeam()
{
	EnemyTeam.Empty();
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EnemyTeam);
}
