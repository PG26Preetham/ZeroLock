// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Abilities/GameplayAbility.h"
#include "ZL_HUD_AbilityIcon.generated.h"

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
	void Setup(const UBaseGameplayAbility* abilityToBindTo ,FGameplayAbilitySpec* InSpec , FGameplayAbilitySpecHandle SpecHandle);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AbilityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AbilityDescription;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> AbilityIcon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr< const UBaseGameplayAbility> AbilityToBindTo;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AZeroLockCharacter> Hero;

	bool alreadyRegstered =false;

	
	FGameplayAbilitySpec* AbilitySpec;

	FGameplayAbilitySpecHandle AbilitySpecHandleToStore;

protected:
	virtual void NativeOnInitialized() override;
	void AbilityActivated(UGameplayAbility* GameplayAbility);
	void OnCooldownTagChanged(FGameplayTag GameplayTag, int I);
	virtual void AddToDelegate();

	virtual void AbilityActivatedEvent();
};
