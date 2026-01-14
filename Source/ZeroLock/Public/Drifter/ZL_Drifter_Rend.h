//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BasePlayAnimation_AndDo.h"
#include "ZL_Drifter_Rend.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Drifter_Rend : public UZL_BasePlayAnimation_AndDo
{
	GENERATED_BODY()

	public:
	UZL_Drifter_Rend();

	virtual void OnAnimationPointTrigger() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float ConeRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float ConeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float ConeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConeIstance")
	float BonusTargetDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FScalableFloat BonusDamage;


	
};
