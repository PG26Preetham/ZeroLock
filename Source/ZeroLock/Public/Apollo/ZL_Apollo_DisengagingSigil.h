// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BasePlayAnimation_AndDo.h"
#include "ZL_Apollo_DisengagingSigil.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Apollo_DisengagingSigil : public UZL_BasePlayAnimation_AndDo
{
	GENERATED_BODY()
	
public:
	UZL_Apollo_DisengagingSigil();
	
	virtual void OnAnimationPointTrigger() override;
	
};
