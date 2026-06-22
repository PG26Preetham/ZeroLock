// Copyright Preetham Mukundan (C) 2026


#include "EOS/ZL_EOS_SubSystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ZeroLock/ZeroLock.h"


UZL_EOS_SubSystem::UZL_EOS_SubSystem()
{
}

void UZL_EOS_SubSystem::Login()
{
	/*
    Tutorial 2: This function will access the EOS OSS via the OSS identity interface to log first into Epic Account Services, and then into Epic Game Services.
    It will bind a delegate to handle the callback event once login call succeeeds or fails. 
    All functions that access the OSS will have this structure: 1-Get OSS interface, 2-Bind delegate for callback and 3-Call OSS interface function (which will call the correspongin EOS OSS function)
    */
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface(); // This is the generic OSS interface that will access the EOS OSS.
 
    // If you're logged in, don't try to login again.
    // This can happen if your player travels to a dedicated server or different maps as BeginPlay() will be called each time.
    
    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
 
    if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
    {
        return; 
    }
    
    /* This binds a delegate so we can run our function when the callback completes. 0 represents the player number.
    You should parametrize this Login function and pass the parameter here for splitscreen. 
    */
    LoginDelegateHandle = 
	Identity->AddOnLoginCompleteDelegate_Handle(
        0,
        FOnLoginCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleLoginCompleted));
 
    // Grab command line parameters. If empty call hardcoded login function - Hardcoded login function useful for Play In Editor. 
    FString AuthType; 
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);
 
    if (!AuthType.IsEmpty()) //If parameter is NOT empty we can autologin.
    {
        /* 
        In most situations you will want to automatically log a player in using the parameters passed via CLI.
        For example, using the exchange code for the Epic Games Store.
        */
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 
      
        if (!Identity->AutoLogin(0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate.
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }
    }
    else
    {
        /* 
        Fallback if the CLI parameters are empty.Useful for PIE.
        The type here could be developer if using the DevAuthTool, ExchangeCode if the game is launched via the Epic Games Launcher, etc...
        */
        FOnlineAccountCredentials Credentials("AccountPortal","", "");
 
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 
        
        if (!Identity->Login(0, Credentials))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate. 
            Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }        
    }
}

void UZL_EOS_SubSystem::ServerTravelAfterPlayersJoin()
{
    GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen"));
}

void UZL_EOS_SubSystem::UpdateLobbyConnectionString()
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    if (!Subsystem) return;

    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    if (!Session.IsValid()) return;

    FOnlineSessionSettings* Settings = Session->GetSessionSettings(LobbyName);
    if (!Settings) return;

    // Get the current map name
    FString CurrentMap = GetWorld()->GetMapName();

    // Ask the OSS for the correct connection string
    FString NewConnectString;
    if (!Session->GetResolvedConnectString(LobbyName, NewConnectString))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to resolve new connect string after travel"));
        return;
    }

    // Update metadata
    Settings->Set(SETTING_MAPNAME, CurrentMap, EOnlineDataAdvertisementType::ViaOnlineService);
    Settings->Set(TEXT("CONNECT_STRING"), NewConnectString, EOnlineDataAdvertisementType::ViaOnlineService);

    // Push update to EOS
    Session->UpdateSession(LobbyName, *Settings, true);

    ZLOG("Updated URL");
    UE_LOG(LogTemp, Log, TEXT("Updated EOS lobby: Map=%s Connect=%s"),
        *CurrentMap, *NewConnectString);


}

void UZL_EOS_SubSystem::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                             const FString& Error)
{
    /*
    Tutorial 2: This function handles the callback from logging in. You should not proceed with any EOS features until this function is called.
    This function will remove the delegate that was bound in the Login() function.
    */
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (bWasSuccessful)
    {
        ZLOG("Login");
        UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
        UE_LOG(LogTemp, Log, TEXT("Searching for a session..."));
        // Maybe via button or player action? Maybe add parameters here
        FindSessions(FName("Zerolock"), FString("Preetham")); 
    }
    else //Login failed
    {
        // If your game is online only, you may want to return an errror to the user and return to a menu that uses a different GameMode/PlayerController.
 
        UE_LOG(LogTemp, Warning, TEXT("EOS login failed.")); //Print sign in failure in logs as a warning.
    }
 
    Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
    LoginDelegateHandle.Reset();
}

void UZL_EOS_SubSystem::FindSessions(FName SearchKey, FString SearchValue)
{
    // Tutorial 4: This function will find our EOS Session that was created by our DedicatedServer.
    // Tutorial 7: This function will find our EOS lobby. Note that at the OSS layer we are using a Session that is marked as a lobby.  Code is similar with minor tweaks
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();
 
    // Remove the default search parameters that FOnlineSessionSearch sets up.
   
 
    Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals); // Seach using our Key/Value pair
#if P2PMODE
    Search->bIsLanQuery = false;
    Search->MaxSearchResults = 20;
    Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
#endif
    FindSessionsDelegateHandle =
        Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleFindSessionsCompleted,
            Search));
#if P2PMODE
    UE_LOG(LogTemp, Log, TEXT("Finding lobby."));
    ZLOG("FindingLobbies");
#else 
    UE_LOG(LogTemp, Log, TEXT("Finding session."));
#endif 
 
    if (!Session->FindSessions(0, Search))
    {
#if P2PMODE
        UE_LOG(LogTemp, Log, TEXT("Finding lobby failed."));
        ZLOG("Finding lobby failed");
#else 
        UE_LOG(LogTemp, Warning, TEXT("Finding session failed."));
#endif
        // Clear our handle and reset the delegate. 
        Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
        FindSessionsDelegateHandle.Reset();
    }
}

void UZL_EOS_SubSystem::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    // Tutorial 4: This function is triggered via the callback we set in FindSession once the session is found (or there is a failure). 
    // Tutorial 7: This function will triggered via the callback we set in FindSession once the lobby is found (or there is a failure). Finding the lobby here has the similar code as finding a session.  
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
    if (bWasSuccessful)
    {
        // added code here to not run into issues when searching for sessions is succesfull, but the number of sessions is 0
        if (Search->SearchResults.Num() == 0)
        {
#if P2PMODE
            ZLOG("Creating lobby");
            // If we're in P2P mode and we can't find a lobby on startup, create one. 
            CreateLobby(FName("Zerolock"), FString("Preetham"));
#endif
            return; 
        }
#if P2PMODE
        UE_LOG(LogTemp, Log, TEXT("Found lobby."));
        ZLOG("Found lobby");
#else 
        UE_LOG(LogTemp, Warning, TEXT("Found session."));
#endif 
        for (auto SessionInSearchResult : Search->SearchResults)
        {
            // Typically you want to check if the session is valid before joining. There is a bug in the EOS OSS where IsValid() returns false when the session is created on a DS. 
            // Instead of customizing the engine for this tutorial, we're simply not checking if the session is valid. The code below should go in this if statement once the bug is fixed. 
            /*
            if (SessionInSearchResult.IsValid()) 
            {
 
                
            }
            */
           
            //Ensure the connection string is resolvable and store the info in ConnectString and in SessionToJoin
            if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
            {
                SessionToJoin = &SessionInSearchResult; 
            }
 
            // For this course we will join the first session found automatically. Usually you would loop through all the sessions and determine which one is best to join. 
            break;            
        }
        JoinSession();  
    }
    else
    {
#if P2PMODE
        UE_LOG(LogTemp, Log, TEXT("Find lobby failed."));
        ZLOG("Find lobby failed");
#else 
        UE_LOG(LogTemp, Warning, TEXT("Find session failed."));
#endif 
    }
 
    // Clear our handle and reset the delegate. 
    Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
    FindSessionsDelegateHandle.Reset();
}

void UZL_EOS_SubSystem::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{ // Tutorial 4: This function is triggered via the callback we set in JoinSession once the session is joined (or there is a failure)
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
#if P2PMODE
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Error, TEXT("Join session was success"));
        if (Session.IsValid())
        {
            FString XConnectString;
            // Get the connection string from the session interface
            if (Session->GetResolvedConnectString(SessionName, XConnectString))
            {
                // Get the first local player controller
                APlayerController* PC = GetWorld()->GetFirstPlayerController();
                
                if (PC)
                {
                    // Call ClientTravel on the Player Controller
                   PC->ClientTravel(XConnectString, ETravelType::TRAVEL_Absolute);
                    
                    // Now that we are in/moving, setup notifications
                    SetupNotifications(); 
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to find PlayerController for ClientTravel!"));
                }
            }
        }// Setup our listeners for lobby event notifications
    }
#else
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("Joined session."));
        if (GEngine)
        {
            // For the purposes of this tutorial overriding the ConnectString to point to localhost as we are testing locally. In a real game no need to override. Make sure you can connect over UDP to the ip:port of your server!
            ConnectString = "127.0.0.1:7777";  
            FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
            FString DedicatedServerJoinError; 
            auto DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError); 
            if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is: %s"), *DedicatedServerJoinError); 
            }
 
            // To be thorough here you should modify your derived UGameInstance to handle the NetworkError and TravelError events. 
            // As we are testing locally, and for the purposes of keeping this tutorial simple, this is omitted. 
        }
    }
#endif
 
    // Clear our handle and reset the delegate. 
    Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
    JoinSessionDelegateHandle.Reset();
}

void UZL_EOS_SubSystem::JoinSession()
{
   
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
    JoinSessionDelegateHandle = 
        Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleJoinSessionCompleted));
#if P2PMODE
    UE_LOG(LogTemp, Log, TEXT("Joining Lobby."));
#else 
    UE_LOG(LogTemp, Log, TEXT("Joining session."));
#endif 
    if (!Session->JoinSession(0, "SessionName", *SessionToJoin))
    {
#if P2PMODE
        UE_LOG(LogTemp, Log, TEXT("Join Lobby failed."));
#else 
        UE_LOG(LogTemp, Log, TEXT("Join session failed."));
#endif 
 
        // Clear our handle and reset the delegate. 
        Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
        JoinSessionDelegateHandle.Reset();
    } 
}
#if P2PMODE
void UZL_EOS_SubSystem::CreateLobby(FName KeyName, FString KeyValue)
{
    
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
 
    CreateLobbyDelegateHandle =
        Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
            this,
            &ThisClass::HandleCreateLobbyCompleted));
 
    TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
    SessionSettings->NumPublicConnections = 2; //We will test our sessions with 2 players to keep things simple
    SessionSettings->bShouldAdvertise = true; //This creates a public match and will be searchable.
    SessionSettings->bUsesPresence = true;   //No presence on dedicated server. This requires a local user.
    SessionSettings->bAllowJoinViaPresence = true;
    SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
    SessionSettings->bAllowInvites = false;    //Allow inviting players into session. This requires presence and a local user. 
    SessionSettings->bAllowJoinInProgress = false; //Once the session is started, no one can join.
    SessionSettings->bIsDedicated = false; //Session created on dedicated server.
    SessionSettings->bUseLobbiesIfAvailable = true; //For P2P we will use a lobby instead of a session
    SessionSettings->bUseLobbiesVoiceChatIfAvailable = true; //We will also enable voice
    SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
    SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));
 
    UE_LOG(LogTemp, Log, TEXT("Creating Lobby..."));
    ZLOG("Creating Lobby...");
 
    if (!Session->CreateSession(0, LobbyName, *SessionSettings))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
    }
}

void UZL_EOS_SubSystem::HandleCreateLobbyCompleted(FName XLobbyName, bool bWasSuccessful)
{
    // Tutorial 7: Callback function: This is called once our lobby is created
 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *XLobbyName.ToString());
        FString s = XLobbyName.ToString();
        ZLOG("LobbyCreated");
        ZLOG(s);
        FString Map = "/Game/Content/ThirdPerson/Maps/ThirdPersonMap?listen"; //Hardcoding map name here, should be passed by parameter
        FURL TravelURL;
        TravelURL.Map = Map;
       GetWorld()->Listen(TravelURL);
       //GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen"));
        SetupNotifications();  
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
    }
    
    Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
    CreateLobbyDelegateHandle.Reset();
}

void UZL_EOS_SubSystem::HandleParticipantJoined(FName SessionName, const FUniqueNetId& ParticipantId)
{
    UE_LOG(LogTemp, Log, TEXT("A player has joined Lobby: %s"), *LobbyName.ToString()); 
    ZLOG("PlayerJoined");
}

void UZL_EOS_SubSystem::HandleParticipantLeft(FName Name, const FUniqueNetId& UniqueNetId,
    EOnSessionParticipantLeftReason OnSessionParticipantLeftReason)
{
    UE_LOG(LogTemp, Log, TEXT("A player has left Lobby: %s"), *Name.ToString());
    ZLOG("PlayerLeft");
}


void UZL_EOS_SubSystem::SetupNotifications()
{
    
    // Tutorial 7: EOS Lobbies are great as there are notifications sent for our backend 
    // when there are changes to lobbies (ex: Participant Joins/Leaves, lobby data is updated, etc...) 
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            // 1. Handle when a participant JOINS the lobby
            Session->AddOnSessionParticipantJoinedDelegate_Handle(
                FOnSessionParticipantJoinedDelegate::CreateUObject(this, &ThisClass::HandleParticipantJoined)
            ); 

            // 2. Handle when a participant LEAVES the lobby
            Session->AddOnSessionParticipantLeftDelegate_Handle(
                FOnSessionParticipantLeftDelegate::CreateUObject(this, &ThisClass::HandleParticipantLeft)
            );
            
            // Note: If you also need to track when a member's data is updated, 
            // you can use FOnSessionParticipantSettingsUpdatedDelegate here as well.
        }
    }
    
}

void UZL_EOS_SubSystem::HandleParticipantChanged(FName EOSLobbyName, const FUniqueNetId& NetId, bool bJoined)
{
    // Tutorial 7: Callback function called when participants join/leave. 
    if (bJoined)
    {
        UE_LOG(LogTemp, Log, TEXT("A player has joined Lobby: %s"), *LobbyName.ToString()); 
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("A player has left Lobby: %s"), *LobbyName.ToString());
    }
}
#endif
