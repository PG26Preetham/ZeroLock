// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Zero_BaseGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLock.h"
#ifndef SEARCH_PRESENCE
#define SEARCH_PRESENCE FName(TEXT("SESSION_PRESENCE"))
#endif
void UZero_BaseGameInstance::Login()
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface(); 
    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
 
    if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
    {
        ZLOG("Already Logged IN");
        return; 
    }
    
  
    LoginDelegateHandle = 
	Identity->AddOnLoginCompleteDelegate_Handle(
        0,
        FOnLoginCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleLoginCompleted));
 
       FString AuthType; 
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);
 
    if (!AuthType.IsEmpty()) 
    {
       
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); 
      
        if (!Identity->AutoLogin(0))
        {
            ZLOG("Failed to login... ");
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); 
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }
    }
    else
    {
       
        FOnlineAccountCredentials Credentials("AccountPortal","", "");

        ZLOG("Logging into EOS...");
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); 
        if (!Identity->Login(0, Credentials))
        {
            ZLOG("Failed to login...");
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... "));
            Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }        
    }
}

void UZero_BaseGameInstance::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	if (OnLoginResult.IsBound())
	{
		OnLoginResult.Broadcast(bWasSuccessful);
	}
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (bWasSuccessful)
	{
		ZLOG_COLOR_TIME("Login callback completed!",FColor::Emerald,30);
		UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));;

		FString Name = Identity->GetPlayerNickname(0);
		
		InitSessionsDelegates();
	}
	else //Login failed
	{
			ZLOG_COLOR_TIME("EOS login failed.",FColor::Emerald,30);
		UE_LOG(LogTemp, Warning, TEXT("EOS login failed.")); //Print sign in failure in logs as a warning.
	}
 
	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}

void UZero_BaseGameInstance::Init()
{
	Super::Init();
	   IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (!Subsystem)return;
    
        // Get the Identity Interface
        IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (!Identity.IsValid())
        {
           return;

        }
    
        // Check login status
        ELoginStatus::Type LoginStatus = Identity->GetLoginStatus(0);
        if (LoginStatus == ELoginStatus::LoggedIn)
        {
            HandleLoginCompleted(0,true,*Identity->GetUniquePlayerId(0),"AlreadyLoggedIN");
        }
    
        UE_LOG(LogTemp, Log, TEXT("User is NOT logged in to EOS."));
        return ;
	//Login();
	//InitSessionsDelegates();
}

void UZero_BaseGameInstance::InitSessionsDelegates()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();

	SessionInterface = Subsystem->GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Session Interface invalid"));
		return;
	}
	CreateCompleteHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UZero_BaseGameInstance::OnCreateSessionComplete)
	);

	FindCompleteHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &UZero_BaseGameInstance::OnFindSessionsComplete)
	);

	JoinCompleteHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &UZero_BaseGameInstance::OnJoinSessionComplete)
	);
}

void UZero_BaseGameInstance::HostSession(const FString& SessionName, int32 MaxPlayers)
{
    if (!SessionInterface.IsValid()) return;

    LastSessionName = SessionName;

    // destroy existing session if exists
    if (SessionInterface->GetNamedSession(NAME_GameSession))
    {
        SessionInterface->DestroySession(NAME_GameSession);
    }
	// Bind delegate
	CreateCompleteHandle =
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
			FOnCreateSessionCompleteDelegate::CreateUObject(
				this, &UZero_BaseGameInstance::OnCreateSessionComplete));

    FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = false; 
	Settings.bUsesPresence = true; // Required for EOS Lobbies visibility
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowInvites = true;
	Settings.bShouldAdvertise = true;
	Settings.bUseLobbiesIfAvailable = true; // Use EOS Lobbies interface
	Settings.NumPublicConnections = 4;

	Settings.Set(FName("ZeroLockLobbies"),true, EOnlineDataAdvertisementType::ViaOnlineService);
    Settings.Set(FName("SESSION_NAME"), SessionName, EOnlineDataAdvertisementType::ViaOnlineService);

    SessionInterface->CreateSession(0, *SessionName, Settings);
}


void UZero_BaseGameInstance::CreateLobby(FName KeyName, FString KeyValue) 
{
   // Tutorial 7: Create lobby - this code is similar to creating session, notice that bIsDedicated is false, bUseLobbiesIfAvailable and UseLobbiesVoiceChatIfAvailable is true
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
    CreateLobbyDelegateHandle =
        Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
            this,
            &ThisClass::OnCreateSessionComplete));
 
    TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
    SessionSettings->NumPublicConnections = 2; //We will test our sessions with 2 players to keep things simple
    SessionSettings->bShouldAdvertise = true; //This creates a public match and will be searchable.
    SessionSettings->bUsesPresence = false;   //No presence on dedicated server. This requires a local user.
    SessionSettings->bAllowJoinViaPresence = false;
    SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
    SessionSettings->bAllowInvites = false;    //Allow inviting players into session. This requires presence and a local user. 
    SessionSettings->bAllowJoinInProgress = false; //Once the session is started, no one can join.
    SessionSettings->bIsDedicated = false; //Session created on dedicated server.
    SessionSettings->bUseLobbiesIfAvailable = true; //For P2P we will use a lobby instead of a session
    SessionSettings->bUseLobbiesVoiceChatIfAvailable = true; //We will also enable voice
    SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
    SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));
 
    UE_LOG(LogTemp, Log, TEXT("Creating Lobby..."));
 
    if (!Session->CreateSession(0,FName("Preetham"), *SessionSettings))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
    }
}

void UZero_BaseGameInstance::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
}


void UZero_BaseGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Create Session Completed: %s Success: %d"),
        *SessionName.ToString(), bWasSuccessful);

    if (!bWasSuccessful) return;

    // Travel to lobby
    UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, TEXT("listen"));
  //  FString Map = "Game/Content/ThirdPerson/Maps/ThirdPersonMap?listen"; //Hardcoding map name here, should be passed by parameter
        ///   FURL TravelURL;
        //   TravelURL.Map = Map;
         //  GetWorld()->Listen(TravelURL);
}

void UZero_BaseGameInstance::FindSessions()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return;

	SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

	// Bind delegate
	FOnFindSessionCompleted =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
			FOnFindSessionsCompleteDelegate::CreateUObject(
				this, &UZero_BaseGameInstance::OnFindSessionsComplete));

	// Init search
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(FName("ZeroLockLobbies"),true, EOnlineComparisonOp::Equals);

	// IMPORTANT: no filters → show all lobbies
	// SessionSearch->QuerySettings.Set()   <-- DO NOT add anything

	// Start search
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UZero_BaseGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem) return;

	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FOnFindSessionCompleted);
	}

	UE_LOG(LogTemp, Warning, TEXT("Find Sessions Completed! Success = %d"), bWasSuccessful);

	if (bWasSuccessful)ZLOG("Found Sessions -");
	
	
	if (!bWasSuccessful || !SessionSearch.IsValid() )
	{
		UE_LOG(LogTemp, Error, TEXT("No sessions found."));
		return;
	}
	FString printf =FString::FromInt( SessionSearch->SearchResults.Num());
	ZLOG(printf);
	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
	{
		auto& Result = SessionSearch->SearchResults[i];

		FString LobbyName ="defValue";
		Result.Session.SessionSettings.Get(FName("SESSION_NAME"), LobbyName);
		ZLOG(LobbyName);
		UE_LOG(LogTemp, Warning, TEXT("[%d] Lobby: %s | Host: %s"),
			i,
			*LobbyName,
			*Result.Session.OwningUserName);
	}
	
	FOnlineSessionSearchResult& FirstResult = SessionSearch->SearchResults[0];
	if (!FirstResult.IsValid())
	{
		ZLOG("first session error not valid");
		return;
	}
	if (SessionInterface.IsValid())
	{
		FOnJoinSessionCompleteDelegate JoinDelegate;
		JoinDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UZero_BaseGameInstance::OnJoinSessionComplete);
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinDelegate);

		if (SessionInterface->JoinSession(0, NAME_GameSession, FirstResult))
		{
			ZLOG("Joining first instance");
		}
		else
		{
			ZLOG("Joining failed");
		}
	}

}

void UZero_BaseGameInstance::JoinSessionBP(int32 Index)
{
    if (!SessionInterface.IsValid()) return;
    if (!CachedResults.IsValidIndex(Index)) return;

    SessionInterface->JoinSession(0, NAME_GameSession, CachedResults[Index]);
}

void UZero_BaseGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid())
    {
        OnJoinResult.Broadcast(false);
        return;
    }

    FString ConnString;
    if (!SessionInterface->GetResolvedConnectString(SessionName, ConnString))
    {
        OnJoinResult.Broadcast(false);
        return;
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->ClientTravel(ConnString, TRAVEL_Absolute);
        OnJoinResult.Broadcast(true);
    }
    else
    {
        OnJoinResult.Broadcast(false);
    }
}
