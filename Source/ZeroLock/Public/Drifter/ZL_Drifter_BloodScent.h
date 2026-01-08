//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BasePassive_GameplayAbility.h"
#include "ZL_Drifter_BloodScent.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Drifter_BloodScent : public UBasePassive_GameplayAbility
{
	GENERATED_BODY()
public:
	UZL_Drifter_BloodScent();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	/** Effect applied to isolated enemies (Attribute debuffs + Trail Tag) */
	UPROPERTY(EditAnywhere, Category = "Drifter | GAS")
	TSubclassOf<UGameplayEffect> IsolatedMarkEffectClass;

	/** Infinite Stacking Effect given to the Hero on isolated kills */
	UPROPERTY(EditAnywhere, Category = "Drifter | GAS")
	TSubclassOf<UGameplayEffect> OnKillStackEffectClass;

	/** Radius to detect isolation */
	UPROPERTY(EditAnywhere, Category = "Drifter | Logic")
	float IsolationRadius = 700.f;

	UPROPERTY(EditAnywhere, Category = "Drifter | Stack")
	int32 KillStackReward = 4;
	UPROPERTY(EditAnywhere, Category = "Drifter | Stack")
	int32 AssistStackReward = 1;

	FTimerHandle LogicTimerHandle;

	// --- Internal Logic ---
	void UpdateServerLogic();

	UFUNCTION()
	void OnTargetDied(const FGameplayEventData Payload);

	UFUNCTION()
	void OnTargetAssist(const FGameplayEventData Payload);

	void GetEnemyPawns(TArray<AZeroLockCharacter*>& OutEnemies);

	UPROPERTY()
	TObjectPtr<class USphereComponent> DetectionSphere;

	// Radius for the sphere (Enemies must be inside this to even be considered)
	UPROPERTY(EditAnywhere, Category = "Drifter | Logic")
	float DetectionRadius = 2000.f;

	// Tracked enemies currently inside the sphere
	UPROPERTY()
	TArray<AZeroLockCharacter*> OverlappingEnemies;

	UFUNCTION()
	void OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
