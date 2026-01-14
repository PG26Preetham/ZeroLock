//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_PlayerInfo : public UMVVMViewModelBase
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Data")
	FText PlayerName;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Data")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Data")
	int32 Kills;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Data")
	int32 Deaths;

	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "Data")
	int32 Assists;


	void SetPlayerName(const FText& NewPlayerName);
	void SetIcon(UTexture2D* NewIcon);
	void SetKills(int32 NewKills);
	void SetDeaths(int32 NewDeaths);
	void SetAssists(int32 NewAssists);

	FText GetPlayerName()const{return PlayerName;};
	int32 GetKills()const{return Kills;};
	int32 GetDeaths()const{return Deaths;};
	int32 GetAssists()const{return Assists;};
	UTexture2D* GetIcon()const{return Icon;};
};
