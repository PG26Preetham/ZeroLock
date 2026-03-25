// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZL_EOS_SubSystem.generated.h"

class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_EOS_SubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	UZL_EOS_SubSystem();
	
	UFUNCTION(BlueprintCallable)
	void Login();
 
	//Callback function. This function is ran when signing into EOS Game Services completes. 
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
 
	//Delegate to bind callback event for login. 
	FDelegateHandle LoginDelegateHandle;
	
	// Function to find EOS sessions. Hardcoded attribute key/value pair to keep things simple
	void FindSessions(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");
 
	// Callback function. This function will run when the session is found.
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
 
	//Delegate to bind callback event for when sessions are found.
	FDelegateHandle FindSessionsDelegateHandle;
 
	// This is the connection string for the client to connect to the dedicated server.
	FString ConnectString;
 
	// This is used to store the session to join information from the search. You could pass it as a paramter to JoinSession() instead. 
	FOnlineSessionSearchResult* SessionToJoin;

	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	// Function to join the EOS session. 
	void JoinSession();
 
	// Callback function. This function will run when the session is joined. 
//	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
 
	// Delegate to bind callback event for join session.
	FDelegateHandle JoinSessionDelegateHandle;
	
};
