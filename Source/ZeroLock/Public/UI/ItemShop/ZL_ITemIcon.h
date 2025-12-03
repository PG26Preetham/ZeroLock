// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ZL_ITemIcon.generated.h"

class UImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_ITemIcon : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ItemPrice;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UFUNCTION(BlueprintCallable)
	void SetupFromItem(class UZero_Item_data* ItemData);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
};
