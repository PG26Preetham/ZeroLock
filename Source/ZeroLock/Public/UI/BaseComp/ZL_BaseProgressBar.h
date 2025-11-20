// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_BaseProgressBar.generated.h"

class UImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_BaseProgressBar : public UCommonActivatableWidget
{
	GENERATED_BODY()


	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dynamic Material Instance")
	UMaterialInterface* ParentMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Instance")
	FName DynamicMaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dynamic Material Instance")
	UMaterialInstanceDynamic* DynamicMI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	bool StartLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	float BackgroundOpacity = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	float BarOpacity = 0.8f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Shape")
	float CornerRoundness = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow")
	float GlowMax = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow")
	float GlowOpacity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Glow")
	float GlowSize = 0.95f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	float ShapeOpacity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Shape")
	float PipSizeX = 0.95f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Shape")
	float PipSizeY = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	FLinearColor BackgroundColor = FLinearColor(0.057806f, 0.658375f, 0.737911f, 1.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	FLinearColor ProgressColorStart = FLinearColor(0.0f, 883.05304f, 1000.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	FLinearColor ProgressColorEnd = FLinearColor(0.0f, 36.582256f, 50.0f, 1.0f);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> WidgetImage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> HealthDisplayText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> MaxHealthDisplayText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> BackgroundImage;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;
	virtual void AddToDelegate();
	virtual void InitDynamicMaterialInstance();
	
	UFUNCTION()
	virtual void OnBarValueChanged(float NewValue, float MaxValue);
};
