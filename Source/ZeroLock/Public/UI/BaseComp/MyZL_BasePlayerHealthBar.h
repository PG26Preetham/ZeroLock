// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseComp/ZL_BaseProgressBar.h"
#include "MyZL_BasePlayerHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UMyZL_BasePlayerHealthBar : public UZL_BaseProgressBar
{
	GENERATED_BODY()

public:
	virtual void AddToDelegate() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	FLinearColor AnimationColor = FLinearColor(0.775822f, 0.014444f, 0.116971f, 1.0f);

protected:
	virtual void OnBarValueChanged(float NewValue, float MaxValue) override;
	
};
