//Copyright Preetham Mukundan (C) 2026


#include "Zero_BasePlayerController.h"

#include "Zero_BasePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLockCharacter.h"

void AZero_BasePlayerController::ServerSetSelectedHero_Implementation(TSubclassOf<AZeroLockCharacter> HeroClass)
{
	SelectedHeroClass = HeroClass;
}

void AZero_BasePlayerController::ClientSelectHero(TSubclassOf<AZeroLockCharacter> HeroClass)
{
	ServerSetSelectedHero(HeroClass);
}

void AZero_BasePlayerController::ServerSetStartLocation_Implementation(FVector loc)
{
	SelectedStartLocation = loc;
}

void AZero_BasePlayerController::ClientSetStartLocation(FVector loc)
{
	ServerSetStartLocation(loc);
}

void AZero_BasePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AZero_BasePlayerState* PS = Cast<AZero_BasePlayerState>(PlayerState);
	if (!PS) return;

	if (OnPSInit.IsBound())
	{
		OnPSInit.Broadcast(PS);	
	}
}

void AZero_BasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AZero_BasePlayerState* PS = Cast<AZero_BasePlayerState>(PlayerState))
	{
		if (AZeroLockCharacter* hero = Cast<AZeroLockCharacter>(InPawn))
		{
			if (hero->Icon)
			{
				PS->SetPlayerIconImage(hero->Icon);
			}
		}
	}
	
}


void AZero_BasePlayerController::ShowDamageNumber_Implementation(float DamageAmount,
                                                                 AZeroLockCharacter* TargetCharacter, FGameplayTagContainer DamageNumberTags)
{
	if (IsValid(TargetCharacter))
	{
		TargetCharacter->AddDamageNumber(DamageAmount, DamageNumberTags);
	}
}

bool AZero_BasePlayerController::ShowDamageNumber_Validate(float DamageAmount, AZeroLockCharacter* TargetCharacter,
	FGameplayTagContainer DamageNumberTags)
{
	return true;
}

void AZero_BasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZero_BasePlayerController,SelectedHeroClass);
	DOREPLIFETIME(AZero_BasePlayerController,SelectedStartLocation);
}

