// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Zero_BasePlayerController.generated.h"


class AZero_BasePlayerState;
class AZeroLockCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, AZero_BasePlayerState*, PS);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated)
	TSubclassOf<AZeroLockCharacter> SelectedHeroClass;
	UPROPERTY(Replicated)
	FVector SelectedStartLocation;

	UFUNCTION(Server, Reliable)
	void ServerSetSelectedHero(TSubclassOf<AZeroLockCharacter> HeroClass);

	void ClientSelectHero(TSubclassOf<AZeroLockCharacter> HeroClass);

	UFUNCTION(Server, Reliable)
	void ServerSetStartLocation(FVector loc);

	void ClientSetStartLocation(FVector loc);

	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPSInit;

	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
