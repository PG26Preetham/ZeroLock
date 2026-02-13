//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZL_ItemTooltipWidget.generated.h"


class UImage;
class UCommonTextBlock;
class UZero_Item_data;

UCLASS()
class ZEROLOCK_API UZL_ItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupFromItem(class UZero_Item_data* ItemData);

protected:

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon; 

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UCommonBorder* BackgroundBorder;


	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class UCommonBorderStyle> SpiritBorderStyle;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class UCommonBorderStyle> WeaponBorderStyle;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class UCommonBorderStyle> VitalityBorderStyle;
};
