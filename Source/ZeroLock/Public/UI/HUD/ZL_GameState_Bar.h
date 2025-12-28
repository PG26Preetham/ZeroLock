// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Zero_BasePlayerState.h"
#include "ZL_GameState_Bar.generated.h"

class UZL_PlayerInfoBox;
class UZL_VM_PlayerInfo;
class UHorizontalBox;
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
	TObjectPtr<UHorizontalBox>  AllyTeamList;
	UPROPERTY(BlueprintReadOnly ,Category ="TeamList",meta=(BindWidget))
	TObjectPtr<UHorizontalBox>  EnemyTeamList;
	

	UFUNCTION(BlueprintCallable)
	void OnAllyTeamChanged(const TArray<UZL_VM_PlayerInfo*>& NewTeam);

	UFUNCTION(BlueprintCallable)
	void OnEnemyTeamChanged(const TArray<UZL_VM_PlayerInfo*>& NewTeam);


	UPROPERTY(EditDefaultsOnly,Category="CharacterIcon")
	TSubclassOf<UUserWidget> PlayerIconClass;
};
