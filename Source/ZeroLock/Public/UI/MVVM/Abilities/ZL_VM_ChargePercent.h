// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_VM_ChargePercent.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VM_ChargePercent : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float MaxChargeTime;
	
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float PerfectMax;
	
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float PerfectMin;
	
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	bool bPerfect;
	
	UPROPERTY(BlueprintReadOnly,FieldNotify,Getter,Setter,meta=(AllowPrivateAccess=true))
	float PerfectCharge;
	
public:
	float GetMaxChargeTime()const {return MaxChargeTime;}
	float GetPerfectMax()const {return PerfectMax;}
	float GetPerfectMin()const {return PerfectMin;}
	bool GetbPerfect()const {return bPerfect;}
	float GetPerfectCharge()const {return PerfectCharge;}
	
	void SetMaxChargeTime(const float newMaxChargeTime);
	void SetPerfectMax(const float newPerfectMax);
	void SetPerfectMin(const float newPerfectMin);
	void SetbPerfect(const bool newPerfectCharge);
	void SetPerfectCharge(const float newPerfectCharge);
	
};
