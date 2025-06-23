// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Tasks/GAST_MeleeMoveTo.h"

#include "ZeroBaseCharacterMovementComp.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLockCharacter.h"

UGAST_MeleeMoveTo::UGAST_MeleeMoveTo(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;
	SpeedOfMovement =100.0f;
}


void UGAST_MeleeMoveTo::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
	TimeMoveStarted = GetWorld()->GetTimeSeconds();
	TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
}

UGAST_MeleeMoveTo* UGAST_MeleeMoveTo::MeleeToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	float Duration , float Speed)
{
	UGAST_MeleeMoveTo* MyObj = NewAbilityTask<UGAST_MeleeMoveTo>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}
	
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->SpeedOfMovement = FMath::Max(Speed, 100.0f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;

	return MyObj;
}

void UGAST_MeleeMoveTo::MeleeHit()
{
	bIsFinished = true;
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnMeleeMoveFinished.Broadcast();
	}
	EndTask();
}

void UGAST_MeleeMoveTo::Activate()
{
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(MyActor);
		if (MyCharacter)
		{
			UZeroBaseCharacterMovementComp* CharMoveComp = Cast<UZeroBaseCharacterMovementComp>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->MeleeHitDelegate.AddDynamic(this,&UGAST_MeleeMoveTo::MeleeHit);
			}
		}
	}
}

void UGAST_MeleeMoveTo::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(MyActor);
		if (MyCharacter)
		{
			UZeroBaseCharacterMovementComp* CharMoveComp = Cast<UZeroBaseCharacterMovementComp>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(MOVE_Custom,CMOVE_Melee);
			}
		}


		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			
			//MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnMeleeMoveFinished.Broadcast();
				}
				EndTask();
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UGAST_MeleeMoveTo::OnDestroy(bool AbilityIsEnding)
{
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(MyActor);
		if (MyCharacter)
		{
			UZeroBaseCharacterMovementComp* CharMoveComp = Cast<UZeroBaseCharacterMovementComp>(MyCharacter->GetMovementComponent());
			if (CharMoveComp && CharMoveComp->MovementMode == MOVE_Custom)
			{
				CharMoveComp->SetMovementMode(MOVE_Falling);
			}
		}
	}

	Super::OnDestroy(AbilityIsEnding);
}
void UGAST_MeleeMoveTo::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGAST_MeleeMoveTo, StartLocation);
	DOREPLIFETIME(UGAST_MeleeMoveTo, DurationOfMovement);
	DOREPLIFETIME(UGAST_MeleeMoveTo, SpeedOfMovement);
}