// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GAST_WaitTargetData.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UGAST_WaitTargetData : public UAbilityTask_WaitTargetData
{
	GENERATED_BODY()

	virtual void Activate() override;

	virtual void RegisterTargetDataCallbacks() override;
	

public:
	/** Spawns target actor and waits for it to return valid data or to be canceled. */
	UFUNCTION(BlueprintCallable, meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms="Instigator"), Category="Ability|Tasks")
	static UGAST_WaitTargetData* WaitTargetDataX(UGameplayAbility* OwningAbility, FName TaskInstanceName, TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType, TSubclassOf<AGameplayAbilityTargetActor> Class);

};
