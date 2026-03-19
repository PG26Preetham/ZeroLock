// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ZL_WaitChargeRelease_Task.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChargeInit,float, MaxTIme, float, perfectmin, float ,perfectmax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FChargeTickDelegate, float, Progress, bool, bIsPerfect, float, ElapsedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChargeReleaseDelegate, float, TotalTime, bool, bWasPerfect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeEndDelegate,  bool, bWasPerfect);

UCLASS()
class ZEROLOCK_API UZL_WaitChargeRelease_Task : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UZL_WaitChargeRelease_Task* WaitChargeRelease(UGameplayAbility* OwningAbility,UAnimMontage* InChargeMontage, float MaxDuration, float PerfectMin, float PerfectMax);

	UPROPERTY(BlueprintAssignable)
	FChargeTickDelegate OnProgressUpdate;
	
	UPROPERTY(BlueprintAssignable)
	FOnChargeInit OnInit;
	
	UPROPERTY(BlueprintAssignable)
	FChargeEndDelegate OnEnd;
	
	UPROPERTY(BlueprintAssignable)
	FChargeReleaseDelegate OnReleased;

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	void OnInputReleased();
	
	FDelegateHandle ReleaseDelegateHandle;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> ChargeMontage;

	float MaxChargeTime;
	float PerfectWindowMin;
	float PerfectWindowMax;
	float ElapsedTime = 0.f;
	bool bTestInitialState;
};
