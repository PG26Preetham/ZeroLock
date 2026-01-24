// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_DamagePopUpBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_DamagePopUpBase : public UCommonActivatableWidget
{
	GENERATED_BODY()


	public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (BindWidget))
	UCommonTextBlock* MainDamageText;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void AddindividualDamageNumber(float DamageAmount ,int32 TypeID);

	UFUNCTION(BlueprintCallable)
	void AddDamageNumber(float DamageAmount ,int32 TypeID);

	FTimerHandle DamageAddTimer;

	int32 DamageTotalAmount;

	UFUNCTION()
	void ClearTimer();
};
