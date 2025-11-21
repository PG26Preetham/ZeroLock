// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_AbilityIcon.generated.h"

class UBaseGameplayAbility;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_AbilityIcon : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:
	void Setup(const UBaseGameplayAbility* abilityToBindTo);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AbilityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AbilityDescription;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> AbilityIcon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr< const UBaseGameplayAbility> AbilityToBindTo;

protected:
	virtual void NativeOnInitialized() override;
	virtual void AddToDelegate();
};
