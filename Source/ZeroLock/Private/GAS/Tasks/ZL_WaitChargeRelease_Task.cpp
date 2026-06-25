// Copyright Preetham Mukundan (C) 2026


#include "GAS/Tasks/ZL_WaitChargeRelease_Task.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"



UZL_WaitChargeRelease_Task* UZL_WaitChargeRelease_Task::WaitChargeRelease(UGameplayAbility* OwningAbility,
	UAnimMontage* InChargeMontage, float MaxDuration, float PerfectMin, float PerfectMax)
{
	UZL_WaitChargeRelease_Task* MyObj = NewAbilityTask<UZL_WaitChargeRelease_Task>(OwningAbility);
	MyObj->MaxChargeTime = MaxDuration;
	MyObj->PerfectWindowMin = PerfectMin;
	MyObj->PerfectWindowMax = PerfectMax;
	MyObj->bTickingTask = true;
	MyObj->ElapsedTime = 0.0f;
	MyObj->bTestInitialState =true;
	MyObj->ChargeMontage = InChargeMontage;
	return MyObj;
}

void UZL_WaitChargeRelease_Task::Activate()
{
	ElapsedTime = 0;
	OnInit.Broadcast(MaxChargeTime, PerfectWindowMin, PerfectWindowMax);
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
    
	if (ASC && Ability)
	{
		if (ChargeMontage)
		{
			ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), ChargeMontage, 1);
		}
       
		if (bTestInitialState && IsLocallyControlled())
		{
			FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec();
			if (Spec && !Spec->InputPressed)
			{
				LocalInputReleased();
				return;
			}
		}

		if (IsLocallyControlled())
		{
			ReleaseDelegateHandle = ASC->AbilityReplicatedEventDelegate(
				EAbilityGenericReplicatedEvent::InputReleased, 
				GetAbilitySpecHandle(), 
				GetActivationPredictionKey()
			).AddUObject(this, &UZL_WaitChargeRelease_Task::LocalInputReleased);
		}
        
		if (IsForRemoteClient()) 
		{
			TargetDataDelegateHandle = ASC->AbilityTargetDataSetDelegate(
				GetAbilitySpecHandle(), 
				GetActivationPredictionKey()
			).AddUObject(this, &UZL_WaitChargeRelease_Task::OnTargetDataReplicatedCallback);
		}
	}
}

void UZL_WaitChargeRelease_Task::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	ElapsedTime += DeltaTime;

	float Progress = FMath::Clamp(ElapsedTime / MaxChargeTime, 0.f, 1.0f);
	bool bInPerfectWindow = (ElapsedTime >= PerfectWindowMin && ElapsedTime <= PerfectWindowMax);

	OnProgressUpdate.Broadcast(Progress, bInPerfectWindow, ElapsedTime);
    
	float EffectiveMaxCharge = MaxChargeTime;
	if (IsForRemoteClient())
	{
		EffectiveMaxCharge += 1.5f; 
	}

	if (ElapsedTime >= EffectiveMaxCharge)
	{
		if (IsLocallyControlled())
		{
			LocalInputReleased();
		}
		else
		{
			TriggerRelease(MaxChargeTime);
		}
	}
}

void UZL_WaitChargeRelease_Task::OnDestroy(bool bInOwnerFinished)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(ReleaseDelegateHandle);
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(TargetDataDelegateHandle);
		AbilitySystemComponent->CurrentMontageStop(-1);
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UZL_WaitChargeRelease_Task::OnInputReleased()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased,GetAbilitySpecHandle(),GetActivationPredictionKey()).Remove(ReleaseDelegateHandle);
	}

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!Ability || !ASC)
	{
		return;
	}

	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(ReleaseDelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	if (IsPredictingClient())
	{
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}
	
	bool bWasPerfect = (ElapsedTime >= PerfectWindowMin && ElapsedTime <= PerfectWindowMax);
    
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnEnd.Broadcast(bWasPerfect);
		OnReleased.Broadcast(ElapsedTime, bWasPerfect);
	}
    
	EndTask();
}

void UZL_WaitChargeRelease_Task::LocalInputReleased()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC) return;

	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(ReleaseDelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, true);


	FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
	LocationData->TargetLocation.LiteralTransform = FTransform(FVector(ElapsedTime, 0.f, 0.f));
    
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(LocationData);


	ASC->CallServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		ASC->ScopedPredictionKey
	);

	TriggerRelease(ElapsedTime);
}

void UZL_WaitChargeRelease_Task::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data,
	FGameplayTag ActivationTag)
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	}
    
	// Unpack the client's float
	if (Data.Data.Num() > 0)
	{
		if (const FGameplayAbilityTargetData_LocationInfo* LocationInfo = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(Data.Data[0].Get()))
		{
			float ClientReportedTime = LocationInfo->TargetLocation.LiteralTransform.GetLocation().X;
			TriggerRelease(ClientReportedTime);
		}
	}
}

void UZL_WaitChargeRelease_Task::TriggerRelease(float TimeCalculated)
{
	bool bWasPerfect = (TimeCalculated >= PerfectWindowMin && TimeCalculated <= PerfectWindowMax);
    
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnEnd.Broadcast(bWasPerfect);
		OnReleased.Broadcast(TimeCalculated, bWasPerfect);
	}
	EndTask();
}
