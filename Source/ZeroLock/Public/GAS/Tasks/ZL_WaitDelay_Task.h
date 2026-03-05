// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ZL_WaitDelay_Task.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgressUpdate,float,Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinished);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_WaitDelay_Task : public UAbilityTask
{
	GENERATED_BODY()
	
	
public:
	UZL_WaitDelay_Task();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName="Wait Delay With Progress",HidePin="OwningAbility",DefaultToSelf="OwningAbility",BlueprintInternalUseOnly="true"))
	static UZL_WaitDelay_Task* WaitDealyWithProgressBar(UGameplayAbility* OwningAbility,float Duration);
	
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FOnFinished OnFinished;
	
	UPROPERTY(BlueprintAssignable)
	FOnFinished OnStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnFinished OnEnd;
	
	UPROPERTY(BlueprintAssignable)
	FOnProgressUpdate OnProgress;

private:

	float Duration = 0.f;
	float StartTime = 0.f;
};
