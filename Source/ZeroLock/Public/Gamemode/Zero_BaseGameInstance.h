// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include"OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Zero_BaseGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString SessionName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Ping = 0;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionSearchResults, const TArray<FSessionInfo>&, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinResult, bool, bSuccess);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZero_BaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void Login();

	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	FDelegateHandle LoginDelegateHandle;
	virtual void Init() override;
	// -------- UI EVENTS --------
	UPROPERTY(BlueprintAssignable)
	FOnSessionSearchResults OnSessionSearchResults;

	UPROPERTY(BlueprintAssignable)
	FOnJoinResult OnJoinResult;

	UFUNCTION(BlueprintCallable)
	void InitSessionsDelegates();
	
	UFUNCTION(BlueprintCallable)
	void HostSession(const FString& SessionName, int32 MaxPlayers = 4);

	UFUNCTION(BlueprintCallable)
	void FindSessions();

	UFUNCTION(BlueprintCallable)
	void JoinSessionBP(int32 Index);


private:

	// -------- ONLINE SESSION SYSTEM --------
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// Store real search results internally
	TArray<FOnlineSessionSearchResult> CachedResults;

	FString LastSessionName;

	// -------- DELEGATES --------
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle CreateCompleteHandle;
	FDelegateHandle FindCompleteHandle;
	FDelegateHandle JoinCompleteHandle;
};
