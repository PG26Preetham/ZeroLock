// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Apollo_Riposte.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Apollo_Riposte : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UZL_Apollo_Riposte();

	UFUNCTION()
	void OnPlayerHit(AActor* HitActor);
	UFUNCTION()
	void OnFinished();
	UFUNCTION()
	void TestWait();
	UFUNCTION()
	void OnEventRecived(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly,Category = "Value")
	float MaxTime =5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> ParryAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> ParryEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> ResistanceEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> StunEffect;
	
	class UZL_WaitDelay_Task* WaitTimeTask ;
	class UAbilityTask_PlayMontageAndWait* AnimMontageTask ;
};
