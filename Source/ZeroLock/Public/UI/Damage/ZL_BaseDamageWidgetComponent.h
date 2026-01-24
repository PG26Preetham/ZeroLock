// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ZL_BaseDamageWidgetComponent.generated.h"

struct FGameplayTagContainer;
class UZL_DamagePopUpBase;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_BaseDamageWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	TSubclassOf<UZL_DamagePopUpBase> MyWidgetClass;

	UZL_BaseDamageWidgetComponent();


	UFUNCTION()
	void ShowDamageNumber(float Damage, FGameplayTagContainer DamageNumberTags);
	
};
