// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BasePlayAnimation_AndDo.h"
#include "ZL_Lash_Flog.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Lash_Flog : public UZL_BasePlayAnimation_AndDo
{
	GENERATED_BODY()


public:
	UZL_Lash_Flog();

	

	UPROPERTY(EditAnywhere, Category = "Flog | Cone")
	float ConeHeight = 500;
	UPROPERTY(EditAnywhere, Category = "Flog | Cone")
	float ConeAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "Flog | Dam")
	FScalableFloat FlogDamage = 1;
	UPROPERTY(EditAnywhere, Category = "Flog | Dam")
	float FlogHeal = 10;

	virtual void OnAnimationPointTrigger() override;
	virtual void OnAnimationCompleted() override;


	UPROPERTY(EditAnywhere, Category = "Flog | Dam")
	TSubclassOf<UGameplayEffect> FlogEffect;
	
};
