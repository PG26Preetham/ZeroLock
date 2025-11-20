// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_CrosshairBase.generated.h"

class AZeroLockCharacter;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_CrosshairBase : public UCommonActivatableWidget
{
	GENERATED_BODY()




protected:
	UPROPERTY()
	AZeroLockCharacter* Hero;
	
	UFUNCTION()
	virtual void AddToDelegate();

	
};
