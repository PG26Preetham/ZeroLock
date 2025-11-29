// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "ZL_EOSGameSession.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_EOSGameSession : public AGameSession
{
	GENERATED_BODY()


public:

	AZL_EOSGameSession();

	virtual void BeginPlay() override;

	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
	void HandleStartSessionCompleted(FName Name, bool bArg);

	void HandleUnregisterPlayerCompleted(FName Name, const TArray<TSharedRef<const FUniqueNetId>>& Shareds, bool bArg);
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;
	
	void StartSession();
	
	void HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);
	virtual bool ProcessAutoLogin() override;
	
	void HandleCreateSessionCompleted(FName Name, bool bArg);
	// Function to create an EOS session. 
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue");

	void HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	void EndSession();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer) override;
	void HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful);
	void DestroySession();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;

	// Hardcoding the max number of players in a session 
	const int MaxNumberOfPlayersInSession = 10;

	// Variable to keep track of the number of players in a session  
	int NumberOfPlayersInSession = 0;

	bool bSessionExists;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;
};
