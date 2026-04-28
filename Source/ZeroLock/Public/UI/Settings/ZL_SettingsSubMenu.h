// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_SettingsSubMenu.generated.h"

enum class ESettingInteractionType : uint8;
class UToggleBaseRow;
class USubMenuManagerViewModel;
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
	
	UFUNCTION(BlueprintCallable)
	void SetupVM(USubMenuManagerViewModel* VM);
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	UScrollBox* ScrollBox;
	
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TSubclassOf<USliderBaseRow> SliderSubClass;
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TSubclassOf<UToggleBaseRow> ToggleSubClass;
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TSubclassOf<USettingsBaseRow> BaseRowClass;
	
	UPROPERTY()
	USubMenuManagerViewModel* MyVM;
	
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	FName SubMenuName;
	
	UPROPERTY(EditDefaultsOnly,Category ="Subclasses")
	TMap<ESettingInteractionType,TSubclassOf<USettingsBaseRow>> SettingSubClassList;
};
