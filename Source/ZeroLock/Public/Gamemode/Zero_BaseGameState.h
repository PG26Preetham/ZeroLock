// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Zero_BaseGameState.generated.h"
class AZero_BasePlayerState;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamArrayUpdated , AZero_BasePlayerState* , ps);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeUpdated , int , gameTime);

enum class ETeamID : uint8;
//class AZero_BasePlayerState;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamRedScore = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 TeamBlueScore = 0;

	void AddKill(ETeamID TeamID);


	UPROPERTY(ReplicatedUsing= On_RedTeamArray, BlueprintReadOnly)
	TArray<AZero_BasePlayerState*> TeamRedArray;

	UPROPERTY(ReplicatedUsing = On_BlueTeamArray , BlueprintReadOnly)
	TArray<AZero_BasePlayerState*> TeamBlueArray;

	UPROPERTY(ReplicatedUsing=On_RepGameTime, BlueprintReadOnly )
	int32 myGameTime = 0;

	UFUNCTION()
	void AddPSToTeamArray(AZero_BasePlayerState* psToAdd,ETeamID teamToAddto);

	UFUNCTION()
	void IncrementGameTime();

	UFUNCTION()
	void On_BlueTeamArray();

	UFUNCTION()
	void On_RedTeamArray();


	UPROPERTY(BlueprintAssignable)
	FOnTeamArrayUpdated OnBlueTeamArrayUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnTeamArrayUpdated OnRedTeamArrayUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnGameTimeUpdated OnGameTimeUpdated;

	UFUNCTION()
	void On_RepGameTime();

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	

	
};
