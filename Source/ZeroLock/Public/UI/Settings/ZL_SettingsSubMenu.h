// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_SettingsSubMenu.generated.h"

class USettingsBaseRow;
class USliderBaseRow;
class UScrollBox;
class USettingBaseViewModel;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_SettingsSubMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CF_List_To_ScrollBox(TArray<USettingBaseViewModel*> ArrayToWorkWith);
	
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TSubclassOf<USliderBaseRow> SliderSubClass;
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TSubclassOf<USettingsBaseRow> BaseRowClass;
};
