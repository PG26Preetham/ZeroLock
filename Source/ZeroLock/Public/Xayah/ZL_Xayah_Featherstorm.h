// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Xayah_Featherstorm.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Xayah_Featherstorm : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:
	UZL_Xayah_Featherstorm();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration")
	TSubclassOf<class AZL_Xayah_projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration")
	int32 ProjectileCount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration")
	float ConeAngle = 90.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration")
	float DownwardAngle = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration")
	float SpawnRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration|Jitter")
	float AngleJitter = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fan Configuration|Jitter")
	float PositionJitter = 15.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LaunchStrength = 1200.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LaunchPitchAngle = 45.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FireDelay = 0.4f;

	UFUNCTION()
	void FireFeathers();
};
