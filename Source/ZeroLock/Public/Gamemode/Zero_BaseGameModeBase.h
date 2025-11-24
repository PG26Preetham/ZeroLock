// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Zero_BaseGameModeBase.generated.h"

class AZeroLockCharacter;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZero_BaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AZero_BaseGameModeBase();

	UPROPERTY(EditDefaultsOnly, Category="Heroes")
	TSubclassOf<AZeroLockCharacter> DefaultHeroClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	// Death + Respawn
	void HandlePlayerDeath(APawn* DeadPawn, AController* DeadController);
	void RespawnPlayer(AZeroLockCharacter* RespawnHero);

private:
	void AssignTeam(class AZero_BasePlayerState* PS);
	APawn* SpawnPawnDefault(TSubclassOf<APawn> PawnClass, AController* Controller, AActor* StartSpot);

public:
	void Killed(AController* Killer, AController* Victim);

	UPROPERTY(EditAnywhere)
	float AssistWindow = 5.f;

	
};
