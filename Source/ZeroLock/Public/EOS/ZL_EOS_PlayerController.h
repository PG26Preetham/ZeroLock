//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ZL_EOS_PlayerController.generated.h"


class FOnlineSessionSearch;
class FOnlineSessionSearchResult;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_EOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AZL_EOS_PlayerController();

	// Function to sign into EOS Game Services.
	UFUNCTION(BlueprintCallable)
	void Login();
protected:
	// Function called when play begins.
	virtual void BeginPlay();


	// Callback function. This function is ran when signing into EOS Game Services completes. 
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	// Delegate to bind callback event for login. 
	FDelegateHandle LoginDelegateHandle;

	// Function to find EOS sessions. Hardcoded attribute key/value pair to keep things simple.
	void FindSessions(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");

	// Callback function. This function will run when the session is found.
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);

	// Delegate to bind callback event for when sessions are found.
	FDelegateHandle FindSessionsDelegateHandle;

	// This is the connection string for the client to connect to the dedicated server.
	FString ConnectString;

	// This is used to store the session to join information from the search. You could pass it as a paramter to JoinSession() instead. 
	FOnlineSessionSearchResult* SessionToJoin; 

	// Function to join the EOS session. 
	void JoinSession();

	// Callback function. This function will run when the session is joined. 
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Delegate to bind callback event for join session.
	FDelegateHandle JoinSessionDelegateHandle;

	// Function used to enumerate title file storage files and read the 1st file in the returned list of files. 
	void LoadTitleData();

	// Callback function. This function is ran when retrieving all the files to be enumerated from the EOS backend completes. 
	void HandleEnumTitleFilesCompleted(bool bWasSuccessfull, const FString& Error);

	// Delegate to bind callback event for title file storage file enumeration.
	FDelegateHandle EnumTitleFilesDelegateHandle;

	// Callback function. This function is ran when a file has been read and the contents can be retrieved.  
	void HandleReadTitleFileCompleted(bool bWasSuccessfull, const FString& FileName); 

	// Delegate to bind callback event for title file storga file read.
	FDelegateHandle ReadTitleFileDelegateHandle;

	// Function to write to player data storage - called when player quits and game is saved.
	void WritePlayerDataStorage(FString FileName, TArray<uint8> FileData);
	
	// Callback function. This function is ran when writing to player data storage completes.
	void HandleWritePlayerDataStorageCompleted(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName);

	// Delegate to bind callback event for title file storage enumeration.
	FDelegateHandle WritePlayerDataStorageDelegateHandle;

	// Function used to enumerate title file storage files and read the 1st file in the returned list of files. 
	void LoadPlayerData();

	// Callback function. This function is ran when retrieving all the files to be enumerated from the EOS backend completes. 
	void HandleEnumPlayerFilesCompleted(bool bWasSuccessfull, const FUniqueNetId& NetId);

	// Delegate to bind callback event for player file storage enumeration.
	FDelegateHandle EnumPlayerFilesDelegateHandle;

	// Callback function. This function is ran when a player data storage file has been read and the contents can be retrieved.  
	void HandleReadPlayerFileCompleted(bool bWasSuccessfull, const FUniqueNetId& UserId, const FString& FileName);

	// Delegate to bind callback event for player data file storage file read.
	FDelegateHandle ReadPlayerDataFileDelegateHandle;


	// Hardcoded name for the lobby.
	FName LobbyName = "LobbyName";
	// Function to create an EOS session. 
	void CreateLobby(FName KeyName = "KeyName", FString KeyValue = "KeyValue");

	// Callback function. This function will run when creating the session compeletes. 
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);

	// Delegate to bind callback event for session creation.
	FDelegateHandle CreateLobbyDelegateHandle;  

	// Function used to setup our listeners to lobby notification events - example on participant change only.
	void SetupNotifications();

	// Callback function. This function will run when a lobby participant joins / leaves.
	void HandleParticipantChanged(FName EOSLobbyName, const FUniqueNetId& NetId, bool bJoined); 


};
