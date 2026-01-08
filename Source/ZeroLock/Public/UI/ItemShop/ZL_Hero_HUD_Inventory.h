//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_Hero_HUD_Inventory.generated.h"

class UUniformGridPanel;
class USizeBox;
class UCommonTileView;
class UZero_Item_data;
class UZL_ITemIcon;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Hero_HUD_Inventory : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void ItemsChanged(TArray<FZeroInventoryItem> InventoryItem);
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UZL_ITemIcon> ItemIconClass;


	UPROPERTY(EditAnywhere,meta=(BindWidget))
	UUniformGridPanel* GridTileView;

	UPROPERTY(EditAnywhere,meta=(BindWidget))
	USizeBox* SizeBox;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SingleElementSquareLength =150.0f;


	UPROPERTY(EditAnywhere)
	TArray<UZL_ITemIcon*> Slots;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UZero_Item_data* DefaultItem;

	
	
};
