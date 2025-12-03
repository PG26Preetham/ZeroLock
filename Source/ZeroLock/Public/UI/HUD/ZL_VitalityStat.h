// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_VitalityStat.generated.h"


class UZL_Stat_Box;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VitalityStat : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	//Health
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> MaxHealth;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> HealingBonus;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> HealingReduction;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> HealthRegen;
	
	virtual void NativeOnInitialized() override;

	void SetupDelegates();
};
