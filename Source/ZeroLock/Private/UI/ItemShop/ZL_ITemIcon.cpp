//Copyright Preetham Mukundan (C) 2026


#include "UI/ItemShop/ZL_ITemIcon.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "CommonBorder.h"
#include "Components/WidgetInteractionComponent.h"
#include "Items/Zero_Item_data.h"
#include "UI/ItemShop/ZL_ItemTooltipWidget.h"
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
	ItemStates->SetVisibility(ESlateVisibility::Collapsed);
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZero_Item_data* ItemData = Cast<UZero_Item_data>(ListItemObject);
	if (!ItemData) return;

	SetupFromItem(ItemData);

	if (TooltipWidgetClass)
	{

		UZL_ItemTooltipWidget* TooltipWidget = CreateWidget<UZL_ItemTooltipWidget>(
			GetOwningPlayer(), 
			TooltipWidgetClass
		);

		if (TooltipWidget)
		{
			TooltipWidget->SetupFromItem(ItemData);
			
			SetToolTip(TooltipWidget);
		}
	}
	
}


void UZL_ITemIcon::SetOnItemPurchased()
{
	ItemIcon->SetOpacity(0.5f);
	ItemCurrentState = EItemState::Sold;

	ItemStates->SetText(FText::FromString("Sold"));
	ItemStates->SetVisibility(ESlateVisibility::Visible);
	ZLOG("ItemClicked ItemIcon");
}

void UZL_ITemIcon::SetOnItemSold()
{
	
	ItemStates->SetVisibility(ESlateVisibility::Collapsed);
	ItemIcon->SetOpacity(1.0f);
	ItemCurrentState = EItemState::Default;
	ZLOG("ItemClicked ItemIcon");
}

void UZL_ITemIcon::SetItemCanBeUpgradedTo(bool bCnaBeUpgraded, UZero_Item_data* ItemFrom)
{
	if (ItemCurrentState == EItemState::Sold)return;
	if (bCnaBeUpgraded)
	{
		ItemStates->SetText(FText::FromString("Upgraded"));
		ItemStates->SetVisibility(ESlateVisibility::Visible);
		ItemCurrentState = EItemState::ReadyToUpgrade;
		OnAnimationPlay(true);
		ItemUpgradedFrom = ItemFrom;
		
	}
	else
	{
		ItemStates->SetVisibility(ESlateVisibility::Collapsed);
		ItemCurrentState = EItemState::Default;
		OnAnimationPlay(false);
		ItemUpgradedFrom = ItemFrom;
	}
	
}

void UZL_ITemIcon::SetUpgradedBlocked()
{
	ItemIcon->SetOpacity(0.5f);
	ItemCurrentState = EItemState::Blocked;
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
