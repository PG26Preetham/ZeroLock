// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ItemShopCategory.h"

#include "CommonTileView.h"
#include "Items/Zero_Item_data.h"
#include "UI/ItemShop/ZL_ITemIcon.h"
#include "ZeroLock/ZeroLock.h"

void UZL_ItemShopCategory::AddItemToTier(UZero_Item_data* itemData)
{
	switch (itemData->ItemTierType)
	{
	case EZeroLockItemTier::Tier1:
			addtoLists(itemData,Tier1_Items,Tier1_ItemListView);
		break;
		case EZeroLockItemTier::Tier2:
			addtoLists(itemData,Tier2_Items,Tier2_ItemListView);
		break;
		case EZeroLockItemTier::Tier3:
			addtoLists(itemData,Tier3_Items,Tier3_ItemListView);
		break;
		case EZeroLockItemTier::Tier4:
			addtoLists(itemData,Tier4_Items,Tier4_ItemListView);
		break;
		default:
		    addtoLists(itemData,Tier1_Items,Tier1_ItemListView);
	}
}

void UZL_ItemShopCategory::addtoLists(UZero_Item_data* itemData, TArray<UZero_Item_data*>& items,
	class UCommonTileView*& listToAdd)
{
	items.AddUnique(itemData);
	listToAdd->AddItem(itemData);
}

void UZL_ItemShopCategory::OnItemSelected(UObject* Object)
{
}

void UZL_ItemShopCategory::OnItemSelected1(UObject* Object)
{
	UZero_Item_data* itemData = Cast<UZero_Item_data>(Object);
	if (itemData)
	{
		if (UZL_ITemIcon* icon =Cast<UZL_ITemIcon>(Tier1_ItemListView->GetEntryWidgetFromItem(itemData)))
		{
			icon->SetOnItemPurchased();
			OnItemClickedOn.Broadcast(itemData);
		}
	}
}

void UZL_ItemShopCategory::OnItemSelected2(UObject* Object)
{
	UZero_Item_data* itemData = Cast<UZero_Item_data>(Object);
	if (itemData)
	{
		if (UZL_ITemIcon* icon =Cast<UZL_ITemIcon>(Tier2_ItemListView->GetEntryWidgetFromItem(itemData)))
		{
			icon->SetOnItemPurchased();
			OnItemClickedOn.Broadcast(itemData);
		}
	}
}

void UZL_ItemShopCategory::OnItemSelected3(UObject* Object)
{
	UZero_Item_data* itemData = Cast<UZero_Item_data>(Object);
	if (itemData)
	{
		if (UZL_ITemIcon* icon =Cast<UZL_ITemIcon>(Tier3_ItemListView->GetEntryWidgetFromItem(itemData)))
		{
			icon->SetOnItemPurchased();
			OnItemClickedOn.Broadcast(itemData);
		}
	}
}

void UZL_ItemShopCategory::OnItemSelected4(UObject* Object)
{
	UZero_Item_data* itemData = Cast<UZero_Item_data>(Object);
	if (itemData)
	{
		if (UZL_ITemIcon* icon =Cast<UZL_ITemIcon>(Tier4_ItemListView->GetEntryWidgetFromItem(itemData)))
		{
			icon->SetOnItemPurchased();
			OnItemClickedOn.Broadcast(itemData);
		}
	}
}

void UZL_ItemShopCategory::AddDelegates()
{
	Tier1_ItemListView->OnItemClicked().AddUObject(this,&UZL_ItemShopCategory::OnItemSelected1);
	Tier2_ItemListView->OnItemClicked().AddUObject(this,&UZL_ItemShopCategory::OnItemSelected2);
	Tier3_ItemListView->OnItemClicked().AddUObject(this,&UZL_ItemShopCategory::OnItemSelected3);
	Tier4_ItemListView->OnItemClicked().AddUObject(this,&UZL_ItemShopCategory::OnItemSelected4);
}

void UZL_ItemShopCategory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AddDelegates();
}
