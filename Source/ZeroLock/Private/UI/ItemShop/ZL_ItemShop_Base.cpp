// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ItemShop_Base.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "CommonTileView.h"
#include "Items/Zero_Item_data.h"
#include "ZeroLock/ZeroLock.h"

UZL_ItemShop_Base::UZL_ItemShop_Base()
{
}

void UZL_ItemShop_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LoadItemsAsync();
}

void UZL_ItemShop_Base::LoadItemsAsync()
{
	if (!ItemTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemTable is NULL"));
		return;
	}
	
	TArray<FItemTableRow*> Rows;
	ItemTable->GetAllRows(TEXT("LoadingItems"), Rows);

	if (Rows.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No rows in ItemTable"));
		return;
	}
	
	TArray<FSoftObjectPath> Paths;
	for (FItemTableRow* Row : Rows)
	{
		if (Row && Row->ItemAsset.IsValid())
		{
			// Already loaded asset
			LoadedItems.AddUnique(Row->ItemAsset.Get());
		}
		else if (Row && !Row->ItemAsset.IsNull())
		{
			// Not loaded yet, add to load list
			Paths.Add(Row->ItemAsset.ToSoftObjectPath());
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
		FStreamableDelegate::CreateUObject(this, &UZL_ItemShop_Base::OnItemsLoaded)
	);
}

void UZL_ItemShop_Base::OnItemsLoaded()
{
	ZLOG("Async load all done");
	LoadedItems.Empty();

	TArray<FItemTableRow*> Rows;
	ItemTable->GetAllRows(TEXT("FinishLoading"), Rows);

	for (FItemTableRow* Row : Rows)
	{
		if (!Row || Row->ItemAsset.IsNull()) continue;

		UZero_Item_data* Item = Row->ItemAsset.Get();
		if (Item)
			LoadedItems.AddUnique(Item);
	}

	// Push loaded items into the CommonUI ListView
	if (ItemListView)
		ItemListView->SetListItems(LoadedItems);
	
}
