//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_MainScreen.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(F_MM_BtnClicked);
/**
 * 
 */
class UCommonButtonBase;
UCLASS()
class ZEROLOCK_API UZL_MainScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void PlayBtnClicked();
	UFUNCTION()
	void SettingsBtnClicked();
	UFUNCTION()
	void QuitBtnClicked();
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite,Category="Btn" ,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Play;

	UPROPERTY(BlueprintReadWrite,Category="Btn" ,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Settings;

	UPROPERTY(BlueprintReadWrite,Category="Btn" ,meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Quit;


	UPROPERTY(BlueprintAssignable)
	F_MM_BtnClicked OnPlayClick;
	UPROPERTY(BlueprintAssignable)
	F_MM_BtnClicked OnSettingsClick;
	UPROPERTY(BlueprintAssignable)
	F_MM_BtnClicked OnQuitClick;
	
};
