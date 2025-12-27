// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Zero_BaseGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "CharacterSelector/ZL_CharacterSelectionSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLock.h"

void UZero_BaseGameInstance::Init()
{
	Super::Init();

	// 1. Get the Subsystem (Engine handles creation automatically)
	UZL_CharacterSelectionSubsystem* SelectionSubsystem = GetSubsystem<UZL_CharacterSelectionSubsystem>();

	if (SelectionSubsystem)
	{
		// 2. If a DataTable was assigned in the Editor, start the load
		if (CharacterDataTable)
		{
			SelectionSubsystem->LoadCharacterTable(CharacterDataTable);
			UE_LOG(LogTemp, Log, TEXT("GameInstance: Initialized Character List from DataTable."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameInstance: No CharacterDataTable assigned!"));
		}
	}
}
