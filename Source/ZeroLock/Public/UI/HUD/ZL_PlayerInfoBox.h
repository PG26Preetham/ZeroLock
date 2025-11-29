// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_PlayerInfoBox.generated.h"

class AZero_BasePlayerState;
class UImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_PlayerInfoBox : public UCommonActivatableWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadOnly,Category="ICON",meta = (BindWidget))
	TObjectPtr<UImage> PlayerIcon;

	UPROPERTY(BlueprintReadOnly,Category="ICON",meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> PlayerName;
	
	UPROPERTY(BlueprintReadOnly,Category="Stat",meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Kill;
	UPROPERTY(BlueprintReadOnly,Category="Stat",meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Death;
	UPROPERTY(BlueprintReadOnly,Category="Stat",meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Assist;

	UFUNCTION()
	void KillUpdate(int NewStatValue);

	UFUNCTION()
	void DeathUpdate(int NewStatValue);
	UFUNCTION()
	void AssistUpdate(int NewStatValue);
	
	UFUNCTION(BlueprintCallable)
	void SetDefaultsWithPS(AZero_BasePlayerState* PS);

	UPROPERTY()
	AZero_BasePlayerState* myPS;
	
};
