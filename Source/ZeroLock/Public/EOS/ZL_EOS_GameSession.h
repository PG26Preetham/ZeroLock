// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "ZL_EOS_GameSession.generated.h"

class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_EOS_GameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	FName SessionNameX = "SessionName"; 

	// Hardcoding the max number of players in a session. 
	const int MaxNumberOfPlayersInSession = 2;

	// Variable to keep track of the number of players in a session.  
	int NumberOfPlayersInSession =0;

	// Function to create an EOS session. 
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue");

	// Callback function. This function will run when creating the session compeletes. 
	void HandleCreateSessionCompleted(FName SessionName, bool bWasSuccessful);

	// Delegate to bind callback event for session creation. 
	FDelegateHandle CreateSessionDelegateHandle;

	// Used to keep track if the session exists or not. 
	bool bSessionExists = false;
	
	virtual bool ProcessAutoLogin() override;
	
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
	
	void HandleRegisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	// Delegate to bind callback event for register player. 
	FDelegateHandle RegisterPlayerDelegateHandle;
	
	// Function to unregister our players in the EOS Session.
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);
	
	// Callback function. This function will run when unregistering the player compeletes.
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	// Delegate to bind callback event for unregister player. 
	FDelegateHandle UnregisterPlayerDelegateHandle;

	// Function to start EOS Session. 
	void StartSession(); 
	
	
	// Callback function. This function will run when start session compeletes.
	void HandleStartSessionCompleted(FName SessionName, bool bWasSuccessful); 

	// Delegate to bind callback event for start session. 
	FDelegateHandle StartSessionDelegateHandle;
	
	// Function called when players leave the dedicated server. Trigger UnregisterPlayer from base class and used to End Sesion. 
	virtual void NotifyLogout(const APlayerController* ExitingPlayer) override;
	
	
	// Function to end EOS Session. 
	void EndSession();

	// Callback function. This function will run when end session compeletes.
	void HandleEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	// Delegate to bind callback event for end session. 
	FDelegateHandle EndSessionDelegateHandle;
	
	
	// Function to Destroy EOS Session. 
	void DestroySession(); 

	// Callback function. This function will run when destroy session compeletes.
	void HandleDestroySessionCompleted(FName SessionName, bool bWasSuccessful);

	// Delegate to bind callback event for destroy session. 
	FDelegateHandle DestroySessionDelegateHandle; 
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
};
