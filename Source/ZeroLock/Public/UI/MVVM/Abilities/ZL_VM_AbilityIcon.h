// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_AbilityIcon.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_AbilityIcon : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify,Setter,Getter, Category = "UI")
	UTexture2D* IconTexture;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	float CooldownPercent = 0.f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	FText CooldownText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "UI")
	bool bIsOnCooldown = false;

	void SetIconTexture(UTexture2D* NewIcon) ;
	UTexture2D* GetIconTexture()const  { return IconTexture; }
	void UpdateCooldown(float Remaining, float Duration);
};
