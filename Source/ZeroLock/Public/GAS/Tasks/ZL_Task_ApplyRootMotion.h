// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "ZL_Task_ApplyRootMotion.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConstantForceHitDelegate, AActor*, HitActor);
UCLASS()
class ZEROLOCK_API UZL_Task_ApplyRootMotion : public UAbilityTask_ApplyRootMotionConstantForce
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FConstantForceHitDelegate OnPlayerHit;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UZL_Task_ApplyRootMotion* ApplyRootMotionConstantForceWithHit(UGameplayAbility* OwningAbility,FName ForceNameX,FVector WorldDirectionX,float StrengthX,float DurationX,bool bStopOnHit,ERootMotionFinishVelocityMode FinishVelocityModeX,FVector FinishSetVelocityX,float FinishClampVelocityXX,bool bEnableGravityX);

	virtual void TickTask(float DeltaTime) override;
	virtual void Activate() override;

protected:
	void CheckForPlayerHit();

	bool bStopOnHit;
	
	UPROPERTY()
	TArray<AActor*> HitActors;
	
};
