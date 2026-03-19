//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HUD_PlayerBase.generated.h"

class UZL_AbilityChargePhase;
class UZL_VM_ChargePercent;
class UZL_HUD_Weapon;
class UZL_Abilities_Bar;
class UMyZL_BasePlayerHealthBar;
class UZL_HUD_CrosshairBase;
class UCommonActivatableWidgetSwitcher;
UCLASS()
class ZEROLOCK_API UZL_HUD_PlayerBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UZL_HUD_PlayerBase();


	UPROPERTY(BlueprintReadOnly,Category ="Crosshair", meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> CrosshairSwitcher;


	UPROPERTY(BlueprintReadOnly, Category="Crosshair", meta=(BindWidget))
	TObjectPtr<UZL_HUD_CrosshairBase> GunCrosshair;

	UPROPERTY(BlueprintReadOnly, Category="HealthBar", meta=(BindWidget))
	TObjectPtr<UMyZL_BasePlayerHealthBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category="AbilityBar", meta=(BindWidget))
	TObjectPtr<UZL_Abilities_Bar> AbilityBar;

	UPROPERTY(BlueprintReadOnly, Category="WeaponBar", meta=(BindWidget))
	TObjectPtr<UZL_HUD_Weapon> WeaponBar;


	UFUNCTION()
	void SetCrosshair();
	
	UFUNCTION(BlueprintCallable)
	void CF_ChargePhaseInit(UZL_VM_ChargePercent* NewChargePercent);
	
	UZL_AbilityChargePhase* ZL_AbilityChargePhase;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UZL_AbilityChargePhase> ChargePhaseClass;

private:
	UFUNCTION()
	void AddToDelegate();

	virtual void NativeOnInitialized() override;

	
};
