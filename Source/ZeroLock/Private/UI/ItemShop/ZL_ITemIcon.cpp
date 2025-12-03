// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ITemIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Items/Zero_Item_data.h"

void UZL_ITemIcon::SetupFromItem(class UZero_Item_data* ItemData)
{
	if (!ItemData)return;

	ItemName->SetText(FText::FromName(ItemData->ItemID));
	ItemIcon->SetBrushFromTexture(ItemData->Icon);
	
}

void UZL_ITemIcon::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZero_Item_data* ItemData = Cast<UZero_Item_data>(ListItemObject);
	if (!ItemData) return;

	SetupFromItem(ItemData);
}
