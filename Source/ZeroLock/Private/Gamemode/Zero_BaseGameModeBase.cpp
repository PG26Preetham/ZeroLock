// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Zero_BaseGameModeBase.h"

#include "Zero_BasePlayerController.h"
#include "Zero_BasePlayerState.h"
#include "Gamemode/Zero_BaseGameState.h"
#include "Gamemode/Zero_BasePlayerStart.h"
#include "GAS/BaseCharAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLockCharacter.h"

AZero_BaseGameModeBase::AZero_BaseGameModeBase()
{
	PlayerControllerClass = AZero_BasePlayerController::StaticClass();
	PlayerStateClass = AZero_BasePlayerState::StaticClass();
	GameStateClass = AZero_BaseGameState::StaticClass();
}

void AZero_BaseGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (AZero_BasePlayerState* PS = NewPlayer->GetPlayerState<AZero_BasePlayerState>())
	{
		AssignTeam(PS);
	}
}

AActor* AZero_BaseGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	AZero_BasePlayerState* PS = Player->GetPlayerState<AZero_BasePlayerState>();
	if (!PS)
		return Super::ChoosePlayerStart_Implementation(Player);

	TArray<AActor*> AllStarts;
	UGameplayStatics::GetAllActorsOfClass(this, AZero_BasePlayerStart::StaticClass(), AllStarts);

	// Filter team spawns
	TArray<AZero_BasePlayerStart*> TeamStarts;
	for (AActor* A : AllStarts)
	{
		if (AZero_BasePlayerStart* TS = Cast<AZero_BasePlayerStart>(A))
		{
			if (TS->TeamID == PS->TeamID)
				TeamStarts.Add(TS);
		}
	}

	if (TeamStarts.Num() > 0)
	{
		return TeamStarts[FMath::RandRange(0, TeamStarts.Num() - 1)];
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* AZero_BaseGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(NewPlayer);

	TSubclassOf<APawn> PawnToSpawn = nullptr;

	if (PC && PC->SelectedHeroClass)
	{
		PawnToSpawn = PC->SelectedHeroClass;
	}
	else
	{
		PawnToSpawn = DefaultHeroClass;
	}

	return SpawnPawnDefault(PawnToSpawn, NewPlayer, StartSpot);
}

void AZero_BaseGameModeBase::HandlePlayerDeath(APawn* DeadPawn, AController* DeadController)
{
	if (!DeadController || !DeadPawn) return;

	AZero_BasePlayerState* DeadPS = DeadController->GetPlayerState<AZero_BasePlayerState>();
	if (DeadPS) DeadPS->AddDeath();

	AController* KillerController = DeadPawn->LastHitBy;
	if (KillerController)
	{
		AZero_BasePlayerState* KillerPS = KillerController->GetPlayerState<AZero_BasePlayerState>();
		if (KillerPS)
		{
			KillerPS->AddKill();

			if (AZero_BaseGameState* GS = GetGameState<AZero_BaseGameState>())
				GS->AddKill(KillerPS->TeamID);
		}
	}

	//DeadPawn->Destroy();
	if (AZeroLockCharacter* hero = Cast<AZeroLockCharacter>(DeadPawn))
	{
		hero->Death();
	
		FTimerHandle RespawnTimer;
		FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &AZero_BaseGameModeBase::RespawnPlayer, hero);
		GetWorldTimerManager().SetTimer(RespawnTimer, Del, 3.f, false);
	}
}

void AZero_BaseGameModeBase::RespawnPlayer(AZeroLockCharacter* RespawnHero)
{
	if (!RespawnHero) return;
	AController* PC = RespawnHero->GetController<AController>();
 if (!PC) return;
	AActor* StartSpot = ChoosePlayerStart(PC);
	RespawnHero->SetActorLocation(StartSpot->GetActorLocation());
	RespawnHero->ResetCharacter(StartSpot->GetActorLocation());
}



void AZero_BaseGameModeBase::AssignTeam(class AZero_BasePlayerState* PS)
{
	AZero_BaseGameState* GS = GetGameState<AZero_BaseGameState>();
	if (!GS) return;

	int32 NumTeam0 = 0;
	int32 NumTeam1 = 0;

	for (APlayerState* Player : GS->PlayerArray)
	{
		if (AZero_BasePlayerState* TDMPS = Cast<AZero_BasePlayerState>(Player))
		{
			if (TDMPS->TeamID == 0) NumTeam0++;
			else NumTeam1++;
		}
	}

	PS->TeamID = (NumTeam0 <= NumTeam1) ? 0 : 1;
}

APawn* AZero_BaseGameModeBase::SpawnPawnDefault(TSubclassOf<APawn> PawnClass, AController* Controller,
	AActor* StartSpot)
{
	if (!PawnClass) return nullptr;

	FActorSpawnParameters Params;
	Params.Owner = Controller;

	FVector Loc = StartSpot->GetActorLocation();
	FRotator Rot = StartSpot->GetActorRotation();

	return GetWorld()->SpawnActor<APawn>(PawnClass, Loc, Rot, Params);
}

void AZero_BaseGameModeBase::Killed(AController* Killer, AController* Victim)
{

}
