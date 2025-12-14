// Fill out your copyright notice in the Description page of Project Settings.


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
	DetailCommon->SetVisibility(ESlateVisibility::Collapsed);
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZero_Item_data* ItemData = Cast<UZero_Item_data>(ListItemObject);
	DescriptionText->SetText(FText::FromString(ItemData->Description));
	if (!ItemData) return;

	SetupFromItem(ItemData);

	if (TooltipWidgetClass)
	{
		// 2. Create the Tooltip Widget
		UZL_ItemTooltipWidget* TooltipWidget = CreateWidget<UZL_ItemTooltipWidget>(
			GetOwningPlayer(), 
			TooltipWidgetClass
		);

		if (TooltipWidget)
		{
			// 3. Set the data on the newly created widget
			TooltipWidget->SetupFromItem(ItemData);
            
			// 4. Assign the instantiated widget using SetToolTip (or SetToolTipWidget)
			// UWidget has a SetToolTipText and SetToolTipWidget function.
			// Using the UWidget::SetToolTipWidget:
			SetToolTip(TooltipWidget);
			
            
			// NOTE: There is also a global Blueprint library function that does the same:
			// UWidgetBlueprintLibrary::SetToolTIp(this, TooltipWidget);
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

void UZL_ITemIcon::SetItemCanBeUpgradedTo(bool bCnaBeUpgraded)
{
	if (bCnaBeUpgraded)
	{
		ItemStates->SetText(FText::FromString("Upgraded"));
		ItemStates->SetVisibility(ESlateVisibility::Visible);
		ItemCurrentState = EItemState::ReadyToUpgrade;
		OnAnimationPlay(true);
		
	}
	else
	{
		ItemStates->SetVisibility(ESlateVisibility::Collapsed);
		ItemCurrentState = EItemState::Default;
		OnAnimationPlay(false);
	}
	
}
void UZL_ITemIcon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetRenderScale(FVector2D(1.2,1.2));
	SetRenderTranslation({-5, -5});
	DetailCommon->SetVisibility(ESlateVisibility::Visible);
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
	DetailCommon->SetVisibility(ESlateVisibility::Collapsed);
	SetRenderScale(FVector2D(1,1));
	SetRenderTranslation({0, 0});
	if (NoneHoverStyle)
	{
		BackGroundCommon->SetStyle(NoneHoverStyle);
	}
}
