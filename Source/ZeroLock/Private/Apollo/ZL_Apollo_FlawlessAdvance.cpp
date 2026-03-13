// Copyright Preetham Mukundan (C) 2026


#include "Apollo/ZL_Apollo_FlawlessAdvance.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/Tasks/ZL_WaitChargeRelease_Task.h"
#include "GAS/Tasks/ZL_WaitDelay_Task.h"
#include "ZeroLock/ZeroLockCharacter.h"

UZL_Apollo_FlawlessAdvance::UZL_Apollo_FlawlessAdvance()
{
}



void UZL_Apollo_FlawlessAdvance::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	CurrentLungeCount = 0;
	StartChargePhase();
}

void UZL_Apollo_FlawlessAdvance::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	if (AnimMontageTask && AnimMontageTask->IsActive())
	{
		AnimMontageTask->EndTask();
	}
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero && Hero->GetCharacterMovement())
	{
		Hero->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UZL_Apollo_FlawlessAdvance::OnInitialMoveComplete()
{
	ChargeStartTime = GetWorld()->GetTimeSeconds();
	UZL_WaitChargeRelease_Task* ZLWaitRelease = UZL_WaitChargeRelease_Task::WaitChargeRelease(this,nullptr,MaxLungeDuration,PerfectWindowMin,PerfectWindowMax);
	if (ZLWaitRelease)
	{
		ZLWaitRelease->OnReleased.AddDynamic(this,&UZL_Apollo_FlawlessAdvance::OnChargeReleased);
		ZLWaitRelease->ReadyForActivation();
	}
}

void UZL_Apollo_FlawlessAdvance::StartChargePhase()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero && Hero->GetCharacterMovement())
	{
		InitialMovementMode = Hero->GetCharacterMovement()->MovementMode;
		Hero->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		Hero->GetCharacterMovement()->StopMovementImmediately();
	}
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	
	if (!Hero) return;
	if (AnimMontageTask && AnimMontageTask->IsActive())
	{
		AnimMontageTask->EndTask();
	}
	AnimMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("AnimMontageAndWait"),ChargeMontage);
	AnimMontageTask->ReadyForActivation();
	FVector MoveDir = GetExactInputDirection();
	UAbilityTask_ApplyRootMotionConstantForce* ActiveMovementTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, NAME_None, MoveDir, ChargeVelocity, ChargeTime, false, nullptr,ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.f, false);
	ActiveMovementTask->OnFinish.AddDynamic(this, &UZL_Apollo_FlawlessAdvance::OnInitialMoveComplete);
	ActiveMovementTask->ReadyForActivation();
}
FVector UZL_Apollo_FlawlessAdvance::GetExactInputDirection() const
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero) return FVector::ZeroVector;
	
	APlayerController* PlayerController = Cast<APlayerController>(Hero->GetController());
	if (!PlayerController) return FVector::ZeroVector;
	
	FVector2D MovementVecotor = Hero->GetMoveVector();
	
	const FRotator Rotation = PlayerController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);


	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector CombinedDir = (ForwardDirection * MovementVecotor.Y) + (RightDirection * MovementVecotor.X);

	
	return CombinedDir;
}

void UZL_Apollo_FlawlessAdvance::OnRelease(float TimeHeld)
{
	ZLOG("Released");
	float CleanHoldTime = GetWorld()->GetTimeSeconds() - ChargeStartTime;
	bool bIsPerfect = (CleanHoldTime >= PerfectWindowMin && CleanHoldTime <= PerfectWindowMax);
	

	ExecuteLunge(bIsPerfect);
}

void UZL_Apollo_FlawlessAdvance::OnChargeReleased(float TotalTime, bool bWasPerfect)
{
	ZLOG("Released");
	ExecuteLunge(bWasPerfect);
}

void UZL_Apollo_FlawlessAdvance::ExecuteLunge(bool bIsPerfect)
{
	AZeroLockCharacter* Character = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;
	
	FVector LookDir = Character->GetFollowCamera()->GetForwardVector();
	float FinalVelocity = bIsPerfect ? LungeBurstVelocity * 1.5f : LungeBurstVelocity;
	float CalculatedDistance = (FinalVelocity *0.2f) + ( 200);

	TArray<FHitResult> Hits;
	TArray<AZeroLockCharacter*> Targets;
	TArray<AActor*> Ignored;
	Ignored.Add(GetAvatarActorFromActorInfo());
	
	if (ReverseConeTraceMulti(GetWorld(),GetAvatarActorFromActorInfo()->GetActorLocation(),LookDir.Rotation(),CalculatedDistance, 10.0f, UEngineTypes::ConvertToTraceType(ECC_Pawn),false,Ignored,EDrawDebugTrace::ForDuration,Hits,Targets,true,FLinearColor::Green,FLinearColor::Red,1.5f))
	{
		if (Targets.Num() > 0)
		{
			for (AZeroLockCharacter* villan : Targets)
			{
				Character->GetMyAbilitySystemComp()->ApplySpiritDamage(villan->GetMyAbilitySystemComp(),10);
			}
		}
	}
	
	UAbilityTask_ApplyRootMotionConstantForce* ActiveRootMotionTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, NAME_None, LookDir, FinalVelocity, ChargeTime, false, nullptr,ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.f, false);
    
	ActiveRootMotionTask->OnFinish.AddDynamic(this, &UZL_Apollo_FlawlessAdvance::OnLungeFinished);
	ActiveRootMotionTask->ReadyForActivation();
}

void UZL_Apollo_FlawlessAdvance::OnLungeFinished()
{
	CurrentLungeCount++;
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("ZeroLock.Abilities.MovementLock")));
	}
	if (AnimMontageTask && AnimMontageTask->IsActive())
	{
		AnimMontageTask->EndTask();
	}
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(GetAvatarActorFromActorInfo());
	if (Hero && Hero->GetCharacterMovement())
	{
		Hero->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
	if (CurrentLungeCount < MaxLunges)
	{ 
		UAbilityTask_WaitInputPress* WaitPress = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
		WaitPress->OnPress.AddDynamic(this, &UZL_Apollo_FlawlessAdvance::OnRecastPressed);
		WaitPress->ReadyForActivation();
		if (WaitTimeTask && WaitTimeTask->IsActive())
		{
			WaitTimeTask->OnFinished.RemoveDynamic(this, &UZL_Apollo_FlawlessAdvance::K2_EndAbility);
			WaitTimeTask->EndTask();
		}
		WaitTimeTask = UZL_WaitDelay_Task::WaitDealyWithProgressBar(this,RecastWindow);
		WaitTimeTask->OnProgress.AddDynamic(this,&UZL_Apollo_FlawlessAdvance::UpdateProgressionTimer);
		WaitTimeTask->OnStarted.AddDynamic(this,&UZL_Apollo_FlawlessAdvance::StartProgressionTimer);
		WaitTimeTask->OnEnd.AddDynamic(this,&UZL_Apollo_FlawlessAdvance::StopProgressionTimer);
		WaitTimeTask->OnFinished.AddDynamic(this,&UZL_Apollo_FlawlessAdvance::K2_EndAbility);
		WaitTimeTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UZL_Apollo_FlawlessAdvance::OnRecastPressed(float TimeWaited)
{
	StartChargePhase();
}

void UZL_Apollo_FlawlessAdvance::DetectLungeTargets(const FVector& StartLocation, const FVector& Direction,
	float Distance, bool bIsPerfect)
{
}
