// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Zero_BasePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged,int32,newStatValue);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AZero_BasePlayerState();

	UPROPERTY(ReplicatedUsing = OnRep_Kills)
	int32 Kills = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Assists)
	int32 Assists = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Deaths)
	int32 Deaths = 0;


	UPROPERTY(ReplicatedUsing = OnRep_TeamID, BlueprintReadOnly)
	int32 TeamID = -1;


	// TEAM SETTER (SERVER ONLY)
	void SetTeamID(int32 NewTeamID);
	
	void AddKill();
	void AddDeath();
	void AddAssist();


public:
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnKillsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnAssistsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnDeathsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnTeamChanged;

protected:

	UFUNCTION()
	void OnRep_Kills();

	UFUNCTION()
	void OnRep_Assists();

	UFUNCTION()
	void OnRep_Deaths();

	UFUNCTION()
	void OnRep_TeamID();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
