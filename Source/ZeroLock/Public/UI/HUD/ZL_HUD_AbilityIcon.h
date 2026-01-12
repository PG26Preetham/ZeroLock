//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "Abilities/GameplayAbility.h"
#include "ZL_HUD_AbilityIcon.generated.h"

class UZL_VM_AbilityIcon;
class AZeroLockCharacter;
struct FGameplayAbilitySpec;
class UBaseGameplayAbility;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_AbilityIcon : public UCommonButtonBase
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void SetViewModel(UZL_VM_AbilityIcon* InViewModel);

	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_AbilityIcon* VM_AbilityIcon;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UCommonTextBlock* Ability_StackText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UImage* AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tooltip")
	TSubclassOf<class UZL_AbilityToolTips> TooltipWidgetClass;

	virtual void NativeOnClicked() override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};
