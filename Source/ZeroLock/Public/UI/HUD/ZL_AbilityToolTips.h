// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZL_AbilityToolTips.generated.h"

class UCommonBorder;
class UCommonBorderStyle ;
class UCommonTextBlock;
class UZL_VM_AbilityIcon;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_AbilityToolTips : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void SetViewModel(UZL_VM_AbilityIcon* InViewModel);

	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_AbilityIcon* VM_AbilityIcon;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* AbilityDescription;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* AbilityLevel1Description;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* Abilityname;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonBorder* B_Level1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonBorder* B_Level2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonBorder* B_Level3;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* AbilityLevel2Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* AbilityLevel3Description;

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void VM_AbilityLevelUp(int32 newLevel);
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category = "Styles")
	TSubclassOf<UCommonBorderStyle> DisabledStyle;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category = "Styles")
	TSubclassOf<UCommonBorderStyle> EnabledStyle;
};
