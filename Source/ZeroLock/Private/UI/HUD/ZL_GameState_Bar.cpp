// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_GameState_Bar.h"

#include "Zero_BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ZL_HUD_TeamList_View.h"

void UZL_GameState_Bar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AddDelegates();
}

void UZL_GameState_Bar::AddPS_Delegates(AZero_BasePlayerState* PS)
{
	if (AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OnPSInit.RemoveDynamic(this,&UZL_GameState_Bar::AddPS_Delegates);
	}
	AllyTeamList->SetupAllyTeam(PS);
	EnemyTeamList->SetupEnemyTeam(PS);
	
}

void UZL_GameState_Bar::AddDelegates()
{
	if (AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (PC->HasAuthority())
		{
			AddPS_Delegates(PC->GetPlayerState<AZero_BasePlayerState>());
		}
		PC->OnPSInit.AddUniqueDynamic(this,&UZL_GameState_Bar::AddPS_Delegates);
	}
}
