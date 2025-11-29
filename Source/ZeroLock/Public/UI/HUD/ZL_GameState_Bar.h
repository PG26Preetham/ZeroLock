// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Zero_BasePlayerState.h"
#include "ZL_GameState_Bar.generated.h"

class UZL_HUD_GameTimer;
class UZL_HUD_TeamList_View;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_GameState_Bar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly ,Category ="TeamList",meta=(BindWidget))
	TObjectPtr<UZL_HUD_TeamList_View>  AllyTeamList;
	UPROPERTY(BlueprintReadOnly ,Category ="TeamList",meta=(BindWidget))
	TObjectPtr<UZL_HUD_TeamList_View>  EnemyTeamList;
	UPROPERTY(BlueprintReadOnly ,Category ="TIme",meta=(BindWidget))
	TObjectPtr<UZL_HUD_GameTimer> Timer;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddPS_Delegates(AZero_BasePlayerState* PS);
	
	void AddDelegates();
	
};
