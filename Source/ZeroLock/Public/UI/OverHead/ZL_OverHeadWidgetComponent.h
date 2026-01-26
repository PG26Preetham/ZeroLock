// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ZL_OverHeadWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_OverHeadWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UZL_OverHeadWidgetComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
