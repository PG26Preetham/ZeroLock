// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
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
class ZEROLOCK_API UZL_HUD_AbilityIcon : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void SetViewModel(UZL_VM_AbilityIcon* InViewModel);

	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_AbilityIcon* VM_AbilityIcon;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	UImage* AbilityIcon;
};
