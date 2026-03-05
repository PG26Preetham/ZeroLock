// Copyright Preetham Mukundan (C) 2026


#include "GAS/Tasks/ZL_WaitDelay_Task.h"

UZL_WaitDelay_Task::UZL_WaitDelay_Task()
{
	bTickingTask = true;
}

UZL_WaitDelay_Task* UZL_WaitDelay_Task::WaitDealyWithProgressBar(UGameplayAbility* OwningAbility, float Duration)
{
	UZL_WaitDelay_Task* MyTask = NewAbilityTask<UZL_WaitDelay_Task>(OwningAbility);
	MyTask->Duration = Duration;
	return MyTask;
}

void UZL_WaitDelay_Task::Activate()
{
	StartTime = GetWorld()->GetTimeSeconds();
	OnStarted.Broadcast();
}

void UZL_WaitDelay_Task::TickTask(float DeltaTime)
{
	const float Elapsed = GetWorld()->GetTimeSeconds() - StartTime;
	const float Percent = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);

	OnProgress.Broadcast(Percent);

	if (Elapsed >= Duration)
	{
		OnFinished.Broadcast();
		EndTask();
	}
}

void UZL_WaitDelay_Task::OnDestroy(bool bInOwnerFinished)
{
	OnEnd.Broadcast();
	Super::OnDestroy(bInOwnerFinished);
}
