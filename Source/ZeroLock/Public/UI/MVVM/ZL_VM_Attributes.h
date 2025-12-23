// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_Attributes.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_Attributes : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	float GetHealth() const {return Health;}
	void SetHealth(const float& health);

	float GetMaxHealth() const {return MaxHealth;}
	void SetMaxHealth(const float& maxhealth);

	UFUNCTION(BlueprintPure,FieldNotify)
	const float GetHealthPercentage()const { return Health/MaxHealth;}

private:
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float Health;

	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float MaxHealth;
};
