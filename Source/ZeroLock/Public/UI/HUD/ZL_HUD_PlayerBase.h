// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_PlayerBase.generated.h"

class UZL_HUD_Weapon;
class UZL_Abilities_Bar;
class UMyZL_BasePlayerHealthBar;
class UZL_HUD_CrosshairBase;
class UCommonActivatableWidgetSwitcher;
UCLASS()
class ZEROLOCK_API UZL_HUD_PlayerBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UZL_HUD_PlayerBase();


	UPROPERTY(BlueprintReadOnly,Category ="Crosshair", meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> CrosshairSwitcher;


	UPROPERTY(BlueprintReadOnly, Category="Crosshair", meta=(BindWidget))
	TObjectPtr<UZL_HUD_CrosshairBase> GunCrosshair;

	UPROPERTY(BlueprintReadOnly, Category="HealthBar", meta=(BindWidget))
	TObjectPtr<UMyZL_BasePlayerHealthBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category="AbilityBar", meta=(BindWidget))
	TObjectPtr<UZL_Abilities_Bar> AbilityBar;

	UPROPERTY(BlueprintReadOnly, Category="WeaponBar", meta=(BindWidget))
	TObjectPtr<UZL_HUD_Weapon> WeaponBar;


	UFUNCTION()
	void SetCrosshair();

private:
	UFUNCTION()
	void AddToDelegate();

	virtual void NativeOnInitialized() override;

	
};
