//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_GameTimer.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HUD_GameTimer : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void TimeUpdated(int GameTime);
	
	void AddDelegates();

	FText FormatTimeText(int32 TotalSeconds);


	UPROPERTY(BlueprintReadOnly,Category="Timer",meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TimerTextBlock;
	
};
