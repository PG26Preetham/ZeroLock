// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeroSelector/ZL_CharacterIcon.h"

#include "CharacterSelector/ZL_Character_Data_Asset.h"
#include "Components/Image.h"

void UZL_CharacterIcon::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UZL_Character_Data_Asset* HeroData = Cast<UZL_Character_Data_Asset>(ListItemObject);
	if (!HeroData) return;

	Icon->SetBrushFromTexture(HeroData->Icon);
}
