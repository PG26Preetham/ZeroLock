// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ItemShopCategory.h"

#include "CommonTileView.h"
#include "Items/Zero_Item_data.h"

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
