// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Tasks/GAST_WaitTargetData.h"

void UGAST_WaitTargetData::Activate()
{
	RegisterTargetDataCallbacks();
	// Need to handle case where target actor was passed into task
	if (Ability && (TargetClass == nullptr))
	{
		if (TargetActor)
		{
			AGameplayAbilityTargetActor* SpawnedActor = TargetActor;
			TargetClass = SpawnedActor->GetClass();

			RegisterTargetDataCallbacks();


			if (!IsValid(this))
			{
				return;
			}

			if (ShouldSpawnTargetActor())
			{
				InitializeTargetActor(SpawnedActor);
				FinalizeTargetActor(SpawnedActor);

				// Note that the call to FinalizeTargetActor, this task could finish and our owning ability may be ended.
			}
			else
			{
				TargetActor = nullptr;

				// We may need a better solution here.  We don't know the target actor isn't needed till after it's already been spawned.
				SpawnedActor->Destroy();
				SpawnedActor = nullptr;
			}
		}
		else
		{
			EndTask();
		}
	}
}

void UGAST_WaitTargetData::RegisterTargetDataCallbacks()
{
	Super::RegisterTargetDataCallbacks();
}

UGAST_WaitTargetData* UGAST_WaitTargetData::WaitTargetDataX(UGameplayAbility* OwningAbility, FName TaskInstanceName,
                                                            TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType, TSubclassOf<AGameplayAbilityTargetActor> Class)
{
	UGAST_WaitTargetData* MyObj = NewAbilityTask<UGAST_WaitTargetData>(OwningAbility, TaskInstanceName);		//Register for task list here, providing a given FName as a key
	MyObj->TargetClass = Class;
	MyObj->TargetActor = nullptr;
	MyObj->ConfirmationType = ConfirmationType;
	return MyObj;
}
