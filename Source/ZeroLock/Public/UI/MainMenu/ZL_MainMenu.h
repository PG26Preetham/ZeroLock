//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_MainMenu.generated.h"

class UZL_MainScreen;
class UZL_LoginPage;
class UCommonActivatableWidgetSwitcher;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_MainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,Category="MainMenu",meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> MainSwitcher;


	UPROPERTY(BlueprintReadOnly,Category="Menu",meta=(BindWidget))
	TObjectPtr<UZL_LoginPage> LoginPage;

	UPROPERTY(BlueprintReadOnly,Category="Menu",meta=(BindWidget))
	TObjectPtr<UZL_MainScreen> MainPage;

	UFUNCTION()
	void ShowLobbyScreen();


	UFUNCTION()
	void ShowSettingsScreen();
	
	UFUNCTION()
	void LoginDone();
	void LoginSetup();
	void MainMenuSetup();
	
	virtual void NativeOnInitialized() override;
	
};
