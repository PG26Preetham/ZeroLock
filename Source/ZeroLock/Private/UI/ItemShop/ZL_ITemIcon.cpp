// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemShop/ZL_ITemIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "CommonBorder.h"
#include "Components/WidgetInteractionComponent.h"
#include "Items/Zero_Item_data.h"
#include "ZeroLock/ZeroLock.h"

void UZL_ITemIcon::SetupFromItem(class UZero_Item_data* ItemData)
{
	if (!ItemData)return;
	ItemDataToStore = ItemData; 
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

void UZL_ITemIcon::SetOnItemPurchased()
{
	ItemIcon->SetOpacity(0.5f);
	ZLOG("ItemClicked ItemIcon");
}

void UZL_ITemIcon::SetOnItemSold()
{
}

void UZL_ITemIcon::SetItemCanBeUpgradedTo()
{
}
void UZL_ITemIcon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetRenderScale(FVector2D(1.2,1.2));
	SetRenderTranslation({-5, -5});
	if (!NoneHoverStyle)
	{
		NoneHoverStyle=BackGroundCommon->Style;
	}
	if (HoverCBStyle)
	{
		BackGroundCommon->SetStyle(HoverCBStyle);
	}

}

void UZL_ITemIcon::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetRenderScale(FVector2D(1,1));
	SetRenderTranslation({0, 0});
	if (NoneHoverStyle)
	{
		BackGroundCommon->SetStyle(NoneHoverStyle);
	}
}
