//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_GameState.generated.h"

class UZL_VM_PlayerInfo;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_GameState : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Teams")
	TArray<UZL_VM_PlayerInfo*> AllyTeam;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Teams")
	TArray<UZL_VM_PlayerInfo*> EnemyTeam;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Time")
	FText FormattedGameTime;


	void SetFormattedGameTime(const FText& NewGameTime);
	void AddAllyTeam(UZL_VM_PlayerInfo* NewAllyTeam);
	void AddEnemyTeam(UZL_VM_PlayerInfo* NewEnemyTeam);

	void EmptyAllyTeam();
	void EmptyEnemyTeam();

	
	
	
};
