// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_BaseProjectileThrowAbility.generated.h"

class ARealProjectile;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_BaseProjectileThrowAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()


public:
	virtual void FireProjectile();

	UFUNCTION()
	virtual void OnEventRecived(FGameplayEventData Payload);

	UFUNCTION()
	virtual void MissOnEventRecived(FGameplayEventData Payload);

	class UAbilityTask_WaitGameplayEvent* WaitGameplayEvent;
	class UAbilityTask_WaitGameplayEvent* MissWaitGameplayEvent;

	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AZero_BaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ProjectileHitTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ProjectileMissTag;

	// Server spawns real projectile
	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector const& Location, FRotator const& Rotation,APredictedProjectile* PredProj);



	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<APredictedProjectile> PredictedProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<ARealProjectile> RealProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
