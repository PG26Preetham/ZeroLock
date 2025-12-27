// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelector/ZL_CharacterSelectionSubsystem.h"

#include "CharacterSelector/ZL_CharacterSelectionVM.h"
#include "CharacterSelector/ZL_Character_Data_Asset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Gamemode/Zero_BaseGameInstance.h"
#include "ZeroLock/ZeroLock.h"

void UZL_CharacterSelectionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (!SelectionVM)
	{
		SelectionVM = NewObject<UZl_CharacterSelectionVM>(this);
	}
	if (UZero_BaseGameInstance* GI = Cast<UZero_BaseGameInstance>(GetGameInstance()))
	{
		if (GI->CharacterDataTable)
		{
			LoadCharacterTable(GI->CharacterDataTable);
		}
	}
}

void UZL_CharacterSelectionSubsystem::LoadCharacterTable(UDataTable* Table)
{
	if (!SelectionVM)
	{
		SelectionVM = NewObject<UZl_CharacterSelectionVM>(this);
	}
	if (!SelectionVM) return;
	if (!Table) return;
	InternalTable = Table;

	TArray<FSoftObjectPath> TargetsToLoad;
	// Assuming FHeroTableRow is your struct from previous snippets
	Table->ForeachRow<FHeroTableRow>(TEXT("Loading"), [&](const FName& Key, const FHeroTableRow& Row)
	{
		TargetsToLoad.Add(Row.HeroAsset.ToSoftObjectPath());
	});

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(TargetsToLoad, FStreamableDelegate::CreateUObject(this, &UZL_CharacterSelectionSubsystem::OnAssetsLoaded, TargetsToLoad));
}

void UZL_CharacterSelectionSubsystem::OnAssetsLoaded(TArray<FSoftObjectPath> LoadedPaths)
{
	TArray<TObjectPtr<UZL_Character_Data_Asset>> TempList;
    
	InternalTable->ForeachRow<FHeroTableRow>(TEXT("Populating"), [&](const FName& Key, const FHeroTableRow& Row)
	{
		if (Row.HeroAsset.IsValid())
		{
			TempList.Add(Row.HeroAsset.Get());
		}
	});
	SelectionVM->SetCharacterList(TempList);
}
