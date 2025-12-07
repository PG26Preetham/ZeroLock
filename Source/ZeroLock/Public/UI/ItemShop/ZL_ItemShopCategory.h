// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_ItemShopCategory.generated.h"

class UZero_Item_data;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClickedOn ,UZero_Item_data* ,ItemClickedOn);

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_ItemShopCategory : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UCommonTileView* Tier1_ItemListView;
	UPROPERTY(meta = (BindWidget))
	class UCommonTileView* Tier2_ItemListView;
	UPROPERTY(meta = (BindWidget))
	class UCommonTileView* Tier3_ItemListView;
	UPROPERTY(meta = (BindWidget))
	class UCommonTileView* Tier4_ItemListView;

	UFUNCTION()
	void AddItemToTier(UZero_Item_data* itemData);

	UPROPERTY()
	TArray<UZero_Item_data*> Tier1_Items;
	UPROPERTY()
	TArray<UZero_Item_data*> Tier2_Items;
	UPROPERTY()
	TArray<UZero_Item_data*> Tier3_Items;
	UPROPERTY()
	TArray<UZero_Item_data*> Tier4_Items;

	void addtoLists(UZero_Item_data* itemData, TArray<UZero_Item_data*>& items,class UCommonTileView*& listToAdd);


	UFUNCTION()
	void OnItemSelected(UObject* Object);
	void OnItemSelected1(UObject* Object);
	void OnItemSelected2(UObject* Object);
	void OnItemSelected3(UObject* Object);
	void OnItemSelected4(UObject* Object);
	void AddDelegates();
	virtual void NativeOnInitialized() override;


	UPROPERTY(BlueprintAssignable)
	FOnItemClickedOn OnItemClickedOn;
	
};
