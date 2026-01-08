//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_WeaponStat.generated.h"


class UZL_Stat_Box;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_WeaponStat : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	//WeaponDamage
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> WeaponDamageBonus;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> FlatWeaponDamage;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> WeaponLifeSteal;

	//WeaponInfo
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> MaxAmmo;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> Firerate;

	//Weapon Res
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> WeaponResistance;
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UZL_Stat_Box> WeaponResistanceReduction;

	virtual void NativeOnInitialized() override;

	void SetupDelegates();
};
