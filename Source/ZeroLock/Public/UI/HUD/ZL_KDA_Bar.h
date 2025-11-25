// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_KDA_Bar.generated.h"

class UZL_Stat_Box;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_KDA_Bar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> KillStatBox;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> DeathStatBox;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> AssistStatBox;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void NewValueChanged(int NewStatValue);
	UFUNCTION()
	void AddPS_Delegates(AZero_BasePlayerState* PS);

	void AddDelegates();
	
};
