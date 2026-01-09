// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/Base_GA_TargetActors.h"
#include "ZL_Lash_DeathSlam.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Lash_DeathSlam : public UBase_GA_TargetActors
{
	GENERATED_BODY()
public:
	UZL_Lash_DeathSlam();

	/** Overriding Activate to inject our custom Cylinder logic */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** Time in seconds an enemy must stay in the cylinder to be "Locked" */
	UPROPERTY(EditAnywhere, Category = "Targeting|Cylinder")
	float LockOnThreshold = 1.5f;

	/** Radius of the cylinder */
	UPROPERTY(EditAnywhere, Category = "Targeting|Cylinder")
	float CylinderRadius = 200.0f;

	/** Length/Distance of the cylinder in front of the player */
	UPROPERTY(EditAnywhere, Category = "Targeting|Cylinder")
	float CylinderLength = 600.0f;

	/** Optional: Effect to apply to all successfully locked targets upon confirmation */
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> LockOnEffect;
	TArray<TWeakObjectPtr<AActor>> OutActors;
	/** Override to process the final list of tracked actors */
	virtual void AbilityConfirmedAction(const FGameplayAbilityTargetDataHandle& Data) override;
};
