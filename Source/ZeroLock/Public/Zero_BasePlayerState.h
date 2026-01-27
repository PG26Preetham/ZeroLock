//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZeroLock/ZeroLock.h"
#include "Zero_BasePlayerState.generated.h"

class UZL_VM_PlayerInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, int32, newStatValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamChanged,ETeamID,newStatValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIconChanged,UTexture2D*,Playerimage);
/**
 * 
 */

class UZL_VM_Attributes;
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
	ETeamID TeamID = ETeamID::TeamNull;


	// TEAM SETTER (SERVER ONLY)
	void SetTeamID(ETeamID id_team);
	
	void AddKill();
	void AddDeath();
	void AddAssist();

	// Replicated image/icon (can be UTexture2D, UMaterial, etc.)
	UPROPERTY(ReplicatedUsing=OnRep_PlayerImage, BlueprintReadOnly)
	UTexture2D* PlayerImage;

	// Update the image on the server
	UFUNCTION(BlueprintCallable)
	void SetPlayerIconImage(UTexture2D* NewImage);
	


public:
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnKillsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnAssistsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnDeathsChanged;
	UPROPERTY(BlueprintAssignable)
	FOnTeamChanged OnTeamChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerIconChanged OnPlayerIconChanged;



	UPROPERTY(Transient, BlueprintReadOnly, Category = "MVVM")
	TObjectPtr<UZL_VM_Attributes> CurrentVM;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "MVVM")
	TObjectPtr<UZL_VM_PlayerInfo> PlayerIconVM;

	void SetPlayerIconVM(TObjectPtr<UZL_VM_PlayerInfo> NewVM){PlayerIconVM = NewVM;};

	void SetCurrentVM(UZL_VM_Attributes* InVM);

	UFUNCTION()
	void OnRep_Kills();

	UFUNCTION()
	void OnRep_Assists();

	UFUNCTION()
	void OnRep_Deaths();

	UFUNCTION()
	void OnRep_TeamID();

	UFUNCTION()
	void OnRep_PlayerImage();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
