// Fill out your copyright notice in the Description page of Project Settings.


#include "Zero_BasePlayerController.h"

#include "Net/UnrealNetwork.h"

void AZero_BasePlayerController::ServerSetSelectedHero_Implementation(TSubclassOf<AZeroLockCharacter> HeroClass)
{
	SelectedHeroClass = HeroClass;
}

void AZero_BasePlayerController::ClientSelectHero(TSubclassOf<AZeroLockCharacter> HeroClass)
{
	ServerSetSelectedHero(HeroClass);
}

void AZero_BasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZero_BasePlayerController,SelectedHeroClass);
}
