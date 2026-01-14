//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GAST_MeleeMoveTo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeMoveDelegate);
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UGAST_MeleeMoveTo : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FMeleeMoveDelegate		OnMeleeMoveFinished;

	UGAST_MeleeMoveTo(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;

	/** Move to the specified location, using the vector curve (range 0 - 1) if specified, otherwise the float curve (range 0 - 1) or fallback to linear interpolation */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGAST_MeleeMoveTo* MeleeToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName,float Duration, float Speed);

	UFUNCTION()
	void MeleeHit();
	virtual void Activate() override;

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:

	bool bIsFinished;

	UPROPERTY(Replicated)
	FVector StartLocation;

	//FVector 

	UPROPERTY(Replicated)
	float DurationOfMovement;
	
	UPROPERTY(Replicated)
	float SpeedOfMovement;


	float TimeMoveStarted;

	float TimeMoveWillEnd;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
};
