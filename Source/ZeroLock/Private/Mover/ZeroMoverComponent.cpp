// Copyright Preetham Mukundan (C) 2026


#include "Mover/ZeroMoverComponent.h"

#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/LayeredMoves/BasicLayeredMoves.h"
#include "DefaultMovementSet/LayeredMoves/MultiJumpLayeredMove.h"
#include "Engine/World.h"
#include "Mover/ZeroMovementData.h" 

UZeroMoverComponent::UZeroMoverComponent()
{
    SetIsReplicated(true);
}

void UZeroMoverComponent::OnMoverPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd)
{
    Super::OnMoverPreSimulationTick(TimeStep, InputCmd);

    const FCharacterDefaultInputs* FoundDefaults = InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
    const FZeroMovementInputs* FoundZeroInputs = InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>();

    if (!FoundDefaults || !FoundZeroInputs) return;

    FName CurrentMode = GetMovementModeName();
    HandleCrouching(CurrentMode,*FoundZeroInputs);
    HandleAirJumpTracking(CurrentMode, *FoundZeroInputs);


    if (FoundZeroInputs->bWantsToDash)
    {
        HandleDashInputs(*FoundDefaults, *FoundZeroInputs);
    }
}

void UZeroMoverComponent::HandleAirJumpTracking(const FName& CurrentMode, const FZeroMovementInputs& ZeroInputs)
{
    if (CurrentMode == DefaultModeNames::Walking || CurrentMode == TEXT("Sliding") || CurrentMode == TEXT("WallJumping"))
    {
        LocalAirJumpsUsed = 0;
    }
    if (HandleWallBounceCheck(ZeroInputs,CurrentMode))
    {
        return;
    }
    
    if (ZeroInputs.bCustomJumpJustPressed &&  LocalAirJumpsUsed < MaxAirJumps)
    {
        TSharedPtr<FLayeredMove_MultiJump> JumpMove = MakeShared<FLayeredMove_MultiJump>();
        JumpMove->UpwardsSpeed = 800;
        JumpMove->MaximumInAirJumps = MaxAirJumps;
        JumpMove->MixMode = EMoveMixMode::OverrideVelocity;
        QueueLayeredMove(JumpMove);
        LocalAirJumpsUsed++;
    }
}

void UZeroMoverComponent::HandleCrouching(const FName& CurrentMode, const FZeroMovementInputs& ZeroInputs)
{
    if (CurrentMode != DefaultModeNames::Walking)
    {
        return;
    }
    if (ZeroInputs.bWantsToCrouch)
    {
        Crouch();
    }
    else
    {
        UnCrouch();
    }
}


bool UZeroMoverComponent::HandleWallBounceCheck(const FZeroMovementInputs& ZeroInputs, const FName& CurrentMode)
{
    if (CurrentMode != DefaultModeNames::Falling || !ZeroInputs.bCustomJumpJustPressed)
    {
        return false;
    }
    UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetUpdatedComponent());
    if (!Capsule) return false;

    FVector Start = Capsule->GetComponentLocation();
    float CheckRadius = Capsule->GetScaledCapsuleRadius() + WallBounceTracePadding;
    
    FCollisionShape Sphere = FCollisionShape::MakeSphere(CheckRadius);
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(GetOwner());

    FHitResult Hit;
    bool bHitWall = Capsule->GetWorld()->SweepSingleByProfile(
        Hit, Start, Start, FQuat::Identity, TEXT("BlockAllDynamic"), Sphere, TraceParams
    );
    
    if (bHitWall && FMath::Abs(Hit.Normal.Z) < 0.3f)
    {
        FVector CurrentVelocity = GetVelocity();
        FVector WallNormal = Hit.Normal;
        
        if (FVector::DotProduct(CurrentVelocity, WallNormal) < 0.0f)
        {
            CurrentVelocity = FVector::VectorPlaneProject(CurrentVelocity, WallNormal);
        }
        
        FVector LaunchVelocity = CurrentVelocity;
        LaunchVelocity += WallNormal * WallJumpOffForce;
        LaunchVelocity.Z = WallJumpVerticalForce;


        TSharedPtr<FLayeredMove_LinearVelocity> WallBounceMove = MakeShared<FLayeredMove_LinearVelocity>();
        WallBounceMove->Velocity = LaunchVelocity;
        WallBounceMove->DurationMs = 0.15f * 1000.0f; 
        
        WallBounceMove->MixMode = EMoveMixMode::OverrideVelocity; 

        QueueLayeredMove(WallBounceMove);
        return true;
    }
    return false;
}




void UZeroMoverComponent::HandleDashInputs(const FCharacterDefaultInputs& DefaultInputs, const FZeroMovementInputs& ZeroInputs)
{
    TSharedPtr<FLayeredMove_LinearVelocity> DashMove = MakeShared<FLayeredMove_LinearVelocity>();
    
    FVector MoveIntent = DefaultInputs.GetMoveInput();
    FVector DashDirection = MoveIntent.IsNearlyZero() ? ZeroInputs.LookDir.Vector().GetSafeNormal2D() : MoveIntent.GetSafeNormal();
    
    if (DashDirection.IsNearlyZero())
    {
        DashDirection = GetOwner()->GetActorForwardVector();
    }

    DashMove->Velocity = DashDirection * DashSpeed;
    DashMove->DurationMs = DashDuration * 1000.0f;
    DashMove->MixMode = EMoveMixMode::OverrideVelocity;

    QueueLayeredMove(DashMove);
}
