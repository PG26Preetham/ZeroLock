//Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_GameState_Bar.h"
#include "Components/HorizontalBox.h"
#include "UI/HUD/ZL_PlayerInfoBox.h"


void UZL_GameState_Bar::OnAllyTeamChanged(const TArray<UZL_VM_PlayerInfo*>& NewTeam)
{
	AllyTeamList->ClearChildren();
	for (UZL_VM_PlayerInfo* PlayerVM : NewTeam)
	{
		UZL_PlayerInfoBox* NewWidget = CreateWidget<UZL_PlayerInfoBox>(this, PlayerIconClass);
		if (NewWidget)
		{
			NewWidget->SetViewModel(PlayerVM); 
			AllyTeamList->AddChildToHorizontalBox(NewWidget);
		}
	}
}

void UZL_GameState_Bar::OnEnemyTeamChanged(const TArray<UZL_VM_PlayerInfo*>& NewTeam)
{
	EnemyTeamList->ClearChildren();
	for (UZL_VM_PlayerInfo* PlayerVM : NewTeam)
	{
		UZL_PlayerInfoBox* NewWidget = CreateWidget<UZL_PlayerInfoBox>(this, PlayerIconClass);
		if (NewWidget)
		{
			NewWidget->SetViewModel(PlayerVM); 
			EnemyTeamList->AddChildToHorizontalBox(NewWidget);
		}
	}
}
