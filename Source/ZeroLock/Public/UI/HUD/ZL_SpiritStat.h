// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_SpiritStat.generated.h"


class UZL_Stat_Box;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_SpiritStat : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	//SpiritDamage
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> SpiritDamageBonus;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> FlatSpiritDamage;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> SpiritLifeSteal;
	
	//Spirit Res
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> SpiritResistance;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> SpiritResistanceReduction;

	virtual void NativeOnInitialized() override;

	void SetupDelegates();
};
