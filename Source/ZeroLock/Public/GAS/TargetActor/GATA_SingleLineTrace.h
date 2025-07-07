// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "GATA_SingleLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AGATA_SingleLineTrace : public AGameplayAbilityTargetActor_SingleLineTrace
{
	GENERATED_BODY()
public:
	AGATA_SingleLineTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
};
