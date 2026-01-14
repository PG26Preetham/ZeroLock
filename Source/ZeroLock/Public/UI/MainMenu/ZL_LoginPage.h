//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_LoginPage.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZLLoginDone);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_LoginPage : public UCommonActivatableWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly,Category="LoginPage",meta=(BindWidget))
	TObjectPtr<class UCommonButtonBase> LoginBtn;

	UPROPERTY(BlueprintReadOnly,Category="LoginPage",meta=(BindWidget))
	TObjectPtr<class UCommonTextBlock> LoginResultText;

	UFUNCTION()
	void LoginResultRecieved(bool bSuccess);
	UFUNCTION()
	void LoginClicked();
	
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintAssignable)
	FZLLoginDone OnLoginDone;
};
