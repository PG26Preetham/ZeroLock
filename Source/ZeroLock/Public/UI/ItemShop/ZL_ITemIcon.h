// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonBorder.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/WidgetComponent.h"
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
	UImage* ItemIcon;
	UPROPERTY(meta = (BindWidget))
	class UCommonBorder* BackGroundCommon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonBorderStyle> HoverCBStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonBorderStyle> NoneHoverStyle;


	UFUNCTION(BlueprintCallable)
	void SetupFromItem(class UZero_Item_data* ItemData);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UZero_Item_data* ItemDataToStore;
	
	UFUNCTION(BlueprintCallable)
	void SetOnItemPurchased();
	UFUNCTION(BlueprintCallable)
	void SetOnItemSold();
	UFUNCTION(BlueprintCallable)
	void SetItemCanBeUpgradedTo();
	 
	

	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	
};
