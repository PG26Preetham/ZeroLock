// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Abilities_Bar.generated.h"


class UCommonTextBlock;
class AZeroLockCharacter;
class UZL_HUD_AbilityIcon;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Abilities_Bar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UZL_HUD_AbilityIcon> Ability1;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UZL_HUD_AbilityIcon> Ability2;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UZL_HUD_AbilityIcon> Ability3;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UZL_HUD_AbilityIcon> Ultimate;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AbilitiesList;

	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;

	virtual void NativePreConstruct() override;
	
	TMap<EGASAbilityInputID , UZL_HUD_AbilityIcon*> AbilityIconMap;
	TMap<FGameplayTag , UZL_HUD_AbilityIcon*> AbilityTagMap;

	UFUNCTION()
	void GrantIconToAbilities(const UBaseGameplayAbility* AbilitytoAdd,EGASAbilityInputID SlotToAddIn);

	UFUNCTION()
	void GrantIconToAbilityWithTag(const UBaseGameplayAbility* AbilitytoAdd,FGameplayTag TagToAddTO);
	UFUNCTION(BlueprintCallable)
	void GrantIconToAbilitiesX();

	UFUNCTION(BlueprintCallable)
	void GrantIconToAbilitiesNew1();

	UFUNCTION(BlueprintCallable)
	void GrantIconToAbilitiesNew2();
	


	AZeroLockCharacter* Hero;

	UFUNCTION()
	void GrantIconToAbilitiesY(UBaseGameplayAbility* AbilitytoAdd, EGASAbilityInputID SlotToAddIn);
	UFUNCTION()
	void AddDelegates();

	UFUNCTION()
	void InitMap();

	
};
