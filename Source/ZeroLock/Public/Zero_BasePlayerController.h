// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Zero_BasePlayerController.generated.h"

class AZeroLockCharacter;
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

	UFUNCTION(Server, Reliable)
	void ServerSetSelectedHero(TSubclassOf<AZeroLockCharacter> HeroClass);

	void ClientSelectHero(TSubclassOf<AZeroLockCharacter> HeroClass);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
