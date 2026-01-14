//Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/GameStats/ZL_VM_PlayerInfo.h"

void UZL_VM_PlayerInfo::SetPlayerName(const FText& NewPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewPlayerName);
}

void UZL_VM_PlayerInfo::SetIcon(UTexture2D* NewIcon)
{
	UE_MVVM_SET_PROPERTY_VALUE(Icon, NewIcon);
}

void UZL_VM_PlayerInfo::SetKills(int32 NewKills)
{
	UE_MVVM_SET_PROPERTY_VALUE(Kills, NewKills);
}

void UZL_VM_PlayerInfo::SetDeaths(int32 NewDeaths)
{
	UE_MVVM_SET_PROPERTY_VALUE(Deaths, NewDeaths);
}

void UZL_VM_PlayerInfo::SetAssists(int32 NewAssists)
{
	UE_MVVM_SET_PROPERTY_VALUE(Assists, NewAssists);
}
