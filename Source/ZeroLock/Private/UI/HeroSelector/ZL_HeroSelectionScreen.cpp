// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeroSelector/ZL_HeroSelectionScreen.h"

#include "CommonTileView.h"
#include "CharacterSelector/ZL_CharacterSelector_Actor.h"
#include "CharacterSelector/ZL_Character_Data_Asset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ItemShop/ZL_ItemShop_Base.h"
#include "ZeroLock/ZeroLock.h"

UZL_HeroSelectionScreen::UZL_HeroSelectionScreen()
{
}

void UZL_HeroSelectionScreen::HoveredItemChanged(UObject* Object, bool bArg)
{
	if (!bArg)return;
	if (!RenderHeroActor) return;

	UZL_Character_Data_Asset* CharacterData = Cast<UZL_Character_Data_Asset>(Object);
	if (!CharacterData) return;

	RenderHeroActor->SetPreview(CharacterData->DisplaySeletalMesh,CharacterData->DisplayAnimation);
}

void UZL_HeroSelectionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LoadItemsAsync();
	RenderHeroActor= Cast<AZL_CharacterSelector_Actor>(UGameplayStatics::GetActorOfClass(GetWorld(),AZL_CharacterSelector_Actor::StaticClass()));
	
	HeroSelectionList->OnItemIsHoveredChanged().AddUObject(this,&UZL_HeroSelectionScreen::HoveredItemChanged);
}

void UZL_HeroSelectionScreen::LoadItemsAsync()
{
	if (!HeroTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemTable is NULL"));
		return;
	}
	
	TArray<FHeroTableRow*> Rows;
	HeroTable->GetAllRows(TEXT("LoadingItems"), Rows);

	if (Rows.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No rows in ItemTable"));
		return;
	}
	
	TArray<FSoftObjectPath> Paths;
	for (FHeroTableRow* Row : Rows)
	{
		if (Row && Row->HeroAsset.IsValid())
		{
			// Already loaded asset
			LoadedItems.AddUnique(Row->HeroAsset.Get());
		}
		else if (Row && !Row->HeroAsset.IsNull())
		{
			// Not loaded yet, add to load list
			Paths.Add(Row->HeroAsset.ToSoftObjectPath());
		}
	}

	
	if (Paths.Num() == 0)
	{
		OnItemsLoaded();
		return;
	}

	// 4. Async load missing ones
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	Streamable.RequestAsyncLoad(
		Paths,
		FStreamableDelegate::CreateUObject(this, &UZL_HeroSelectionScreen::OnItemsLoaded)
	);
}

void UZL_HeroSelectionScreen::OnItemsLoaded()
{
	ZLOG("Async load all done");
	LoadedItems.Empty();

	TArray<FHeroTableRow*> Rows;
	HeroTable->GetAllRows(TEXT("FinishLoading"), Rows);

	for (FHeroTableRow* Row : Rows)
	{
		if (!Row || Row->HeroAsset.IsNull()) continue;

		UZL_Character_Data_Asset* Item = Row->HeroAsset.Get();
		if (Item)
		{
			LoadedItems.AddUnique(Item);
		}
	}
	// Push loaded items into the CommonUI ListView
	if (HeroSelectionList)
		HeroSelectionList->SetListItems(LoadedItems);
}
