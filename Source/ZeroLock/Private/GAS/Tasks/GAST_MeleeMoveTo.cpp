//Copyright Preetham Mukundan (C) 2026

#include "GAS/Tasks/GAST_MeleeMoveTo.h"
#include "ZeroBaseCharacterMovementComp.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "Net/UnrealNetwork.h"

UGAST_MeleeMoveTo::UGAST_MeleeMoveTo(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bTickingTask = true;
    bSimulatedTask = true;
    bIsFinished = false;
}

UGAST_MeleeMoveTo* UGAST_MeleeMoveTo::MeleeToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration, float Speed)
{
    UGAST_MeleeMoveTo* MyObj = NewAbilityTask<UGAST_MeleeMoveTo>(OwningAbility, TaskInstanceName);
    MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);
    MyObj->SpeedOfMovement = FMath::Max(Speed, 100.0f);
    return MyObj;
}

void UGAST_MeleeMoveTo::Activate()
{
    AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(GetAvatarActor());
    if (MyCharacter)
    {
        UZeroBaseCharacterMovementComp* CharMoveComp = Cast<UZeroBaseCharacterMovementComp>(MyCharacter->GetMovementComponent());
        if (CharMoveComp)
        {
            CharMoveComp->SetMovementMode(MOVE_Custom, CMOVE_Melee);
            CharMoveComp->MeleeHitDelegate.AddDynamic(this, &UGAST_MeleeMoveTo::MeleeHit);
        }
        
        TimeMoveStarted = GetWorld()->GetTimeSeconds();
        TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
    }
}

void UGAST_MeleeMoveTo::TickTask(float DeltaTime)
{
    if (bIsFinished) return;
    Super::TickTask(DeltaTime);

    if (GetWorld()->GetTimeSeconds() >= TimeMoveWillEnd)
    {
        bIsFinished = true;
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnMeleeMoveFinished.Broadcast();
        }
        EndTask();
    }
}

void UGAST_MeleeMoveTo::MeleeHit()
{
    if (bIsFinished) return;
    bIsFinished = true;

    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnMeleeMoveFinished.Broadcast();
    }
    EndTask();
}

void UGAST_MeleeMoveTo::OnDestroy(bool AbilityIsEnding)
{
    AZeroLockCharacter* MyCharacter = Cast<AZeroLockCharacter>(GetAvatarActor());
    if (MyCharacter)
    {
        UZeroBaseCharacterMovementComp* CharMoveComp = Cast<UZeroBaseCharacterMovementComp>(MyCharacter->GetMovementComponent());
        if (CharMoveComp)
        {
            CharMoveComp->MeleeHitDelegate.RemoveDynamic(this, &UGAST_MeleeMoveTo::MeleeHit);
            if (CharMoveComp->CustomMovementMode == CMOVE_Melee)
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
void UGAST_MeleeMoveTo::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
    Super::InitSimulatedTask(InGameplayTasksComponent);
    
    TimeMoveStarted = GetWorld()->GetTimeSeconds();
    TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
}