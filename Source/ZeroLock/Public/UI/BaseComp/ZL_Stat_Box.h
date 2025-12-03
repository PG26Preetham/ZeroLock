// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayEffectTypes.h"
#include "ZL_Stat_Box.generated.h"

class UAttributeSet;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Stat_Box : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> StatValueText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> StatIcon;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> StatName;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void NewValueChanged(int32 newValue);
	UFUNCTION()
	void NewValueChangedFloat(float newValue);

	UFUNCTION()
	void SetIcon(UTexture2D* Icon);
	void AtrributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);


	UFUNCTION()
	void InitGameplayAttribute(FGameplayAttribute AttributeToBind,UAbilitySystemComponent* FromASC,UAttributeSet* FromAS);
};
