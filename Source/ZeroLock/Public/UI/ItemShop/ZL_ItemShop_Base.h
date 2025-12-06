// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_ItemShop_Base.generated.h"


class UCommonButtonBase;
class UZL_ItemShopCategory;
class UCommonActivatableWidgetSwitcher;
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

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void BTN_All_Pressed() const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void BTN_Weapon_Pressed() const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void BTN_Spirit_Pressed() const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void BTN_Vitality_Pressed() const;

	UFUNCTION()
	void AddDelegates();

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> ItemSwitcher;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UZL_ItemShopCategory> SpiritCategory;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UZL_ItemShopCategory> VitalityCategory;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UZL_ItemShopCategory> WeaponCategory;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_All;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Weapon;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Spirit;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Vitality;
	
	UFUNCTION(BlueprintCallable)
	void AddtoCategory(UZero_Item_data* dataItem);


	UFUNCTION(BlueprintCallable)
	void ActiveSetForSwitcher(UWidget* WidgettoActive);
};
