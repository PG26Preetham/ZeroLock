// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ZL_CharacterIcon.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_CharacterIcon : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UImage* BG;

	
};
