// Copyright Preetham Mukundan (C) 2026


#include "GAS/Tasks/ZL_Task_ApplyRootMotion.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "ZeroLock/ZeroLockCharacter.h"


UZL_Task_ApplyRootMotion* UZL_Task_ApplyRootMotion::ApplyRootMotionConstantForceWithHit(UGameplayAbility* OwningAbility,
                                                                                        FName ForceNameX, FVector WorldDirectionX, float StrengthX, float DurationX, bool bStopOnHit,
                                                                                        ERootMotionFinishVelocityMode FinishVelocityModeX, FVector FinishSetVelocityX, float FinishClampVelocityXX,
                                                                                        bool bEnableGravityX)
{
	UZL_Task_ApplyRootMotion* MyObj = NewAbilityTask<UZL_Task_ApplyRootMotion>(OwningAbility);
	
	MyObj->Ability = OwningAbility;
	MyObj->ForceName = ForceNameX;
	MyObj->WorldDirection = WorldDirectionX;
	MyObj->Strength = StrengthX;
	MyObj->Duration = DurationX;
	MyObj->bStopOnHit = bStopOnHit;
MyObj->bEnableGravity = bEnableGravityX;
	MyObj->bIsAdditive = false;
	MyObj->StrengthOverTime = nullptr;
	MyObj->FinishVelocityMode = FinishVelocityModeX;
	MyObj->FinishSetVelocity = FinishSetVelocityX;
	MyObj->FinishClampVelocity = FinishClampVelocityXX;
    
	MyObj->bTickingTask = true;
	return MyObj;
}

void UZL_Task_ApplyRootMotion::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	CheckForPlayerHit();
}

void UZL_Task_ApplyRootMotion::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && ASC->AbilityActorInfo.IsValid())
	{
		AActor* Avatar = ASC->AbilityActorInfo->AvatarActor.Get();
		if (Avatar)
		{
			SharedInitAndApply();
		}
	}

	bTickingTask = true;
	Super::Activate();
}

void UZL_Task_ApplyRootMotion::CheckForPlayerHit()
{
	
	AActor* Avatar = GetAvatarActor();
	if (!Avatar) return;

	AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(Avatar);
	if (!MyCharacter) return;

	if (MyCharacter)
	{
		FCollisionShape Cap = FCollisionShape::MakeSphere(50);
		FVector TraceLocation = MyCharacter->GetActorLocation() + MyCharacter->GetActorForwardVector()*50;
		FVector TraceEndLocation =TraceLocation ;
		FHitResult Hits;
		FQuat RotationQuat = MyCharacter->GetActorRotation().Quaternion();
		if(GetWorld()->SweepSingleByObjectType(Hits,TraceLocation,TraceEndLocation,RotationQuat,ECC_GameTraceChannel1,Cap,MyCharacter->GetIgnoreCharacterParams()))
		{
			
			Avatar->ForceNetUpdate();
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnPlayerHit.Broadcast(Hits.GetActor());
			}
			if (bStopOnHit)
			{
				EndTask();
			}
			
		}
	}
}
