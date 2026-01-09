// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Lash_Grapple.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Lash_Grapple : public UBaseGameplayAbility
{
	GENERATED_BODY()
public:

	UZL_Lash_Grapple();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float ConeHeight = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float ConeAngle =45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple")
	float HeightBoostInGrapple =100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple")
	float GrappleLaunchStrength =100;
};
