// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <rapidjson/internal/meta.h>

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_TeamList_View.generated.h"

enum class ETeamID : uint8;
class UZL_PlayerInfoBox;
class AZero_BasePlayerState;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_TeamList_View : public UCommonActivatableWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Box",meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(BlueprintReadOnly,Category="Teams")
	ETeamID TeamID;
	

	UPROPERTY(EditDefaultsOnly,Category="CharacterIcon")
	TSubclassOf<UZL_PlayerInfoBox> PlayerIconClass;

	UFUNCTION(BlueprintCallable)
	void SetupAllyTeam(AZero_BasePlayerState* ps);
	
	UFUNCTION(BlueprintCallable)
	void SetupEnemyTeam(AZero_BasePlayerState* ps);

	UFUNCTION()
	void AddPlayerToBox(AZero_BasePlayerState* PS);
	
	UFUNCTION()
	void SetupDelegates();
	
};
