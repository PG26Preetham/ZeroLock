//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UITag.h"
#include "ZL_BaseUILayout.generated.h"


class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidgetStack;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_BaseUILayout : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UZL_BaseUILayout();

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
	UFUNCTION(BlueprintCallable)
	void PushWidgetToLayer(FUITag LayerTag, TSubclassOf<UCommonActivatableWidget> Widget);

	UFUNCTION(BlueprintCallable)
	UCommonActivatableWidget* GetActivatedWidgetInLayer(FUITag LayerTag);

	TMap<FUITag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layer;

	UPROPERTY(BlueprintReadOnly, Category="Stack", meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	UPROPERTY(BlueprintReadOnly, Category="Stack", meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	UPROPERTY(BlueprintReadOnly, Category="Stack", meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ItemShopStack;

	UPROPERTY(BlueprintReadOnly, Category="Stack", meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> PopUpStack;
	
private:
	virtual void NativeOnInitialized() override;
	
	void RegisterLayer(FUITag LayerTag, TObjectPtr<UCommonActivatableWidgetContainerBase> LayerStack);
};
