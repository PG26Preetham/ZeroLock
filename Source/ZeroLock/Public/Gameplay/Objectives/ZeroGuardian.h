// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Objectives/ZeroObjective.h"
#include "ZeroGuardian.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZeroGuardian : public AZeroObjective
{
	GENERATED_BODY()

public:

	AZeroGuardian();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.0f, ClampMin = 0.0f), Category = "Guardian")
	float BeamDamage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0.1f, ClampMin = 0.1f), Category = "Guardian")
	float BeamDamageRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1.0f, ClampMin = 1.0f), Category = "Guardian")
	float BeamRange = 360.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 1.0f, ClampMin = 1.0f), Category = "Guardian")
	float PunchRange = 130.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guardian")
	TObjectPtr<USphereComponent> BeamSphere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Guardian")
	TObjectPtr<USphereComponent> PunchSphere;

	virtual void OnConstruction(const FTransform& Transform) override;
};
