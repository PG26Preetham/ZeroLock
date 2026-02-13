//Copyright Preetham Mukundan (C) 2026


#include "UI/ItemShop/ZL_ItemShopCategory.h"

#include "CommonTextBlock.h"
#include "CommonTileView.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Items/Zero_Item_data.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ItemShop/ZL_ITemIcon.h"
#include "ZeroLock/ZeroLock.h"
#include "ZeroLock/ZeroLockCharacter.h"

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

void UZL_ItemShopCategory::OnItemSelected(UObject* Object, class UCommonTileView* TileView)
{
	UZero_Item_data* itemData = Cast<UZero_Item_data>(Object);
	
	if (itemData)
	{
		for (UZero_Item_data* it : itemData->NextItemsToUpgrade)
		{
			ZLOG(it->ItemName);
			
		}
		if (UZL_ITemIcon* icon =Cast<UZL_ITemIcon>(TileView->GetEntryWidgetFromItem(itemData)))
		{
			if (icon->ItemCurrentState == EItemState::Sold)
			{
				
				icon->SetOnItemSold();
				OnItemClickedOn.Broadcast(itemData);
				for (UZero_Item_data* it : itemData->NextItemsToUpgrade)
				{
					ZLOG(it->ItemName);
					FindAndSetToUpgradeOrRemove(it,false,nullptr);
				}
				RecSellFunction(icon->ItemUpgradedFrom);
				
			}
			else if (icon->ItemCurrentState == EItemState::Default)
			{
				if (!InventoryComp) return;
				if (!InventoryComp->HasSlotToBuy()) return;
				icon->SetOnItemPurchased();
				OnItemClickedOn.Broadcast(itemData);

				for (UZero_Item_data* it : itemData->NextItemsToUpgrade)
				{
					ZLOG(it->ItemName);
					FindAndSetToUpgradeOrRemove(it,true,itemData);
				}
			}
			else if (icon->ItemCurrentState == EItemState::ReadyToUpgrade)
			{
				if (icon->ItemUpgradedFrom)
				{
					for (UZero_Item_data* it : icon->ItemUpgradedFrom->NextItemsToUpgrade)
					{
						if (it == itemData)
						{
							FindAndSetToUpgradeOrRemove(it,false,icon->ItemUpgradedFrom);
							continue;
						}
						FindAndSetToUpgradeOrRemove(it,false,nullptr);
					}
					if (UZL_ITemIcon* fromIcon = FindAndReturnIcon(icon->ItemUpgradedFrom))
					{
						fromIcon->SetUpgradedBlocked();
					}
				}
				
				icon->SetOnItemPurchased();
				OnItemClickedOn.Broadcast(itemData);

				for (UZero_Item_data* it : itemData->NextItemsToUpgrade)
				{
					ZLOG(it->ItemName);
					FindAndSetToUpgradeOrRemove(it,true,itemData);
				}

				
			}
			
		}
	}
}



void UZL_ItemShopCategory::OnItemSelected1(UObject* Object)
{
	OnItemSelected(Object,Tier1_ItemListView);
}

void UZL_ItemShopCategory::OnItemSelected2(UObject* Object)
{
	OnItemSelected(Object,Tier2_ItemListView);
}

void UZL_ItemShopCategory::OnItemSelected3(UObject* Object)
{
	OnItemSelected(Object,Tier3_ItemListView);
}

void UZL_ItemShopCategory::OnItemSelected4(UObject* Object)
{
	OnItemSelected(Object,Tier4_ItemListView);
}


void UZL_ItemShopCategory::AddDelegates()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		InventoryComp = Hero->GetInventoryComponent();
	}
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

void UZL_ItemShopCategory::FindAndSetToUpgradeOrRemove(UZero_Item_data* ItemsToSetReadyToUpgrade,
	bool setToUpgradeState, UZero_Item_data* FromItem)
{
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier2_ItemListView->GetEntryWidgetFromItem(ItemsToSetReadyToUpgrade)))
	{
		iconToChange->SetItemCanBeUpgradedTo(setToUpgradeState,FromItem);
	}
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier3_ItemListView->GetEntryWidgetFromItem(ItemsToSetReadyToUpgrade)))
	{
		iconToChange->SetItemCanBeUpgradedTo(setToUpgradeState,FromItem);
	}
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier4_ItemListView->GetEntryWidgetFromItem(ItemsToSetReadyToUpgrade)))
	{
		iconToChange->SetItemCanBeUpgradedTo(setToUpgradeState,FromItem);
	}
}

UZL_ITemIcon* UZL_ItemShopCategory::FindAndReturnIcon(UZero_Item_data* ItemData)
{
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier1_ItemListView->GetEntryWidgetFromItem(ItemData)))
	{
		return iconToChange;
	}
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier2_ItemListView->GetEntryWidgetFromItem(ItemData)))
	{
		return iconToChange;
	}
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier3_ItemListView->GetEntryWidgetFromItem(ItemData)))
	{
		return iconToChange;
	}
	if (UZL_ITemIcon* iconToChange =Cast<UZL_ITemIcon>(Tier4_ItemListView->GetEntryWidgetFromItem(ItemData)))
	{
		return iconToChange;
	}
	return nullptr;
}

void UZL_ItemShopCategory::RecSellFunction(UZero_Item_data* ItemData)
{
	if (!ItemData) return;
	if (UZL_ITemIcon* icon = FindAndReturnIcon(ItemData))
	{
		icon->SetOnItemSold();
		if (icon->ItemUpgradedFrom)
		{
			for (UZero_Item_data* it : icon->ItemUpgradedFrom->NextItemsToUpgrade)
			{
				ZLOG(it->ItemName);
				FindAndSetToUpgradeOrRemove(it,false,nullptr);
			}
			RecSellFunction(icon->ItemUpgradedFrom);
		}
	}
}




