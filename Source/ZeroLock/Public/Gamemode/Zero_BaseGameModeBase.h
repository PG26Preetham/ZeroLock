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

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateGameTime(AZero_BaseGameState* GS);

	FTimerHandle GameTimerHandle;

	/** Hero selection */
	UPROPERTY(EditDefaultsOnly, Category="Heroes")
	TSubclassOf<AZeroLockCharacter> DefaultHeroClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	// Death + Respawn
	void HandlePlayerDeath(AZeroLockCharacter* DeadPawn, AController* DeadController);
	void RespawnPlayer(AController* Controller); // changed to accept Controller (safe)

private:
	void AssignTeam(class AZero_BasePlayerState* PS);
	APawn* SpawnPawnDefault(TSubclassOf<AZeroLockCharacter> PawnClass, AController* Controller, AActor* StartSpot);


public:
	void Killed(AController* Killer, AController* Victim);

	UPROPERTY(EditAnywhere)
	float AssistWindow = 5.f;

	
};
