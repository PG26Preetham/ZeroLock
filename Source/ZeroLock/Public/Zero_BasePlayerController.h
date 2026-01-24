//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Zero_BasePlayerController.generated.h"


struct FGameplayTagContainer;
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


	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AZeroLockCharacter* TargetCharacter, FGameplayTagContainer DamageNumberTags);
	void ShowDamageNumber_Implementation(float DamageAmount, AZeroLockCharacter* TargetCharacter, FGameplayTagContainer DamageNumberTags);
	bool ShowDamageNumber_Validate(float DamageAmount, AZeroLockCharacter* TargetCharacter, FGameplayTagContainer DamageNumberTags);


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
