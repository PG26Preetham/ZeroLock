//Copyright Preetham Mukundan (C) 2026


#include "UI/HeroSelector/ZL_CharacterIcon.h"

#include "CharacterSelector/ZL_Character_Data_Asset.h"
#include "Components/Image.h"
#include "ZeroLock/ZeroLock.h"

void UZL_CharacterIcon::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ZLOG("IconInit");
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZL_Character_Data_Asset* HeroData = Cast<UZL_Character_Data_Asset>(ListItemObject);
	if (!HeroData) return;

	Icon->SetBrushFromTexture(HeroData->Icon);
	//Icon->SetBrushTintColor(HeroData->IconColor);
	BG->SetColorAndOpacity(HeroData->IconColor);
}

void UZL_CharacterIcon::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	Icon->SetRenderScale(FVector2D(1.5,1.5));
}

void UZL_CharacterIcon::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Icon->SetRenderScale(FVector2D(1,1));
}
