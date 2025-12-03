// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_ItemShop_Base.generated.h"


class UZero_Item_data;

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// Pick your DataAsset in the DataTable field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UZero_Item_data> ItemAsset;
};
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_ItemShop_Base : public UCommonActivatableWidget
{
	GENERATED_BODY()

	UZL_ItemShop_Base();

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	UDataTable* ItemTable;

	UPROPERTY()
	TArray<UZero_Item_data*> LoadedItems;
	
	UPROPERTY(meta = (BindWidget))
	class UCommonTileView* ItemListView;

	void LoadItemsAsync();
	void OnItemsLoaded();
	
};
