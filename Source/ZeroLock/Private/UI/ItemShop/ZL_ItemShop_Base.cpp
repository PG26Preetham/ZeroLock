// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ItemShop_Base.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "CommonButtonBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "CommonTileView.h"
#include "Items/Zero_Item_data.h"
#include "Items/Zero_Item_Inventory_Component.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ItemShop/ZL_ITemIcon.h"
#include "UI/ItemShop/ZL_ItemShopCategory.h"
#include "ZeroLock/ZeroLock.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_ItemShop_Base::UZL_ItemShop_Base()
{
}

void UZL_ItemShop_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	LoadItemsAsync();
	AddDelegates();
	ItemSwitcher->SetActiveWidget(ItemListView);
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
		{
			LoadedItems.AddUnique(Item);
			AddtoCategory(Item);
		}
	}
	// Push loaded items into the CommonUI ListView
	if (ItemListView)
		ItemListView->SetListItems(LoadedItems);
	
}

void UZL_ItemShop_Base::BTN_All_Pressed() const
{
	ItemSwitcher->SetActiveWidget(ItemListView);
}

void UZL_ItemShop_Base::BTN_Weapon_Pressed() const
{
	ItemSwitcher->SetActiveWidget(WeaponCategory);
}

void UZL_ItemShop_Base::BTN_Spirit_Pressed() const
{
	ItemSwitcher->SetActiveWidget(SpiritCategory);
}

void UZL_ItemShop_Base::BTN_Vitality_Pressed() const
{
	ItemSwitcher->SetActiveWidget(VitalityCategory);
}


void UZL_ItemShop_Base::AddDelegates()
{
	BTN_All->OnClicked().AddUObject(this,&ThisClass::BTN_All_Pressed);
	BTN_Weapon->OnClicked().AddUObject(this,&ThisClass::BTN_Weapon_Pressed);
	BTN_Spirit->OnClicked().AddUObject(this,&ThisClass::BTN_Spirit_Pressed);
	BTN_Vitality->OnClicked().AddUObject(this,&ThisClass::BTN_Vitality_Pressed);

	SpiritCategory->OnItemClickedOn.AddDynamic(this,&UZL_ItemShop_Base::ItemPressed);
	WeaponCategory->OnItemClickedOn.AddDynamic(this,&UZL_ItemShop_Base::ItemPressed);
	VitalityCategory->OnItemClickedOn.AddDynamic(this,&UZL_ItemShop_Base::ItemPressed);
	
}

void UZL_ItemShop_Base::ItemPressed(UZero_Item_data* Data)
{
	//ZLOG(ItemListView->GetEntryWidgetFromItem(Data)->GetName());
	
	UZL_ITemIcon* Icon = Cast<UZL_ITemIcon>(ItemListView->GetEntryWidgetFromItem(Data));
	if (Icon)
	{
		Icon->SetOnItemPurchased();
	}
	if (!HeroItemComponent)
	{
		HeroItemComponent =Cast<UZero_Item_Inventory_Component>( UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetComponentByClass(UZero_Item_Inventory_Component::StaticClass()));
		if (!HeroItemComponent) return;
		
	}
	HeroItemComponent->ServerBuyItem(Data);
}

void UZL_ItemShop_Base::AddtoCategory(UZero_Item_data* dataItem)
{
	if (dataItem->ItemType == EZeroLockItemType::Spirit)
	{
		SpiritCategory->AddItemToTier(dataItem);
		return;
	}
	else if (dataItem->ItemType == EZeroLockItemType::Vitality)
	{
		VitalityCategory->AddItemToTier(dataItem);
		return;
	}
	else
	{
		WeaponCategory->AddItemToTier(dataItem);
		return;
	}
	
}

void UZL_ItemShop_Base::ActiveSetForSwitcher(UWidget* WidgettoActive)
{
	if (!WidgettoActive)return;
	ItemSwitcher->SetActiveWidget(WidgettoActive);
}
