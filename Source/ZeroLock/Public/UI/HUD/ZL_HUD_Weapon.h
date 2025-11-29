// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_Weapon.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_Weapon : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnValueChanged(float currentAmmo, float MaxAmmo);
	void AddDelegates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AmmoDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> MaxAmmoDisplayText;
	
};
