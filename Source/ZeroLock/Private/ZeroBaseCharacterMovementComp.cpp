//Copyright Preetham Mukundan (C) 2026

#include "ZeroBaseCharacterMovementComp.h"
#include "ZeroLock/ZeroLock.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "ZeroLock/Public/Movement/Zero_ZiplineActor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "VisualLogger/VisualLogger.h"

FVector GServerCharacterLoc[4];
DEFINE_LOG_CATEGORY_STATIC(LogZeroMovement, Log, All);

#pragma region SavedMove
UZeroBaseCharacterMovementComp::FSavedMove_Zero::FSavedMove_Zero() {}

bool UZeroBaseCharacterMovementComp::FSavedMove_Zero::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
    FSavedMove_Zero* NewZeroMove = static_cast<FSavedMove_Zero*>(NewMove.Get());

    if (Saved_bWantsToSprint != NewZeroMove->Saved_bWantsToSprint) return false;
    if (Saved_bWantsToDash != NewZeroMove->Saved_bWantsToDash) return false;
    
    return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::Clear()
{
    FSavedMove_Character::Clear();
    Saved_bWantsToSprint = 0;
    Saved_bWantsToDash = 0;
    Saved_bPrevPressedJump = 0;
    Saved_bPrevWantsToCrouch = 0;
    Saved_bHadAnimRootMotion = 0;
    Saved_bTransitionFinished = 0;

    // Clear Custom State
    Saved_DashDir = FVector::ZeroVector;
    Saved_DashSpeed = 0.0f;
    Saved_DashTimeRemaining = 0.0f;
    Saved_DashExitMode = MOVE_Falling;
    Saved_LeapTargetLoc = FVector::ZeroVector;
    Saved_LeapTimeRemaining = 0.0f;
}

uint8 UZeroBaseCharacterMovementComp::FSavedMove_Zero::GetCompressedFlags() const
{
    uint8 Result = Super::GetCompressedFlags();
    if(Saved_bWantsToSprint) Result |= FLAG_Sprint;
    if(Saved_bWantsToDash) Result |= FLAG_Dash;
    
    if(Saved_bPrevPressedJump) Result |= FLAG_PrevJump;
    if(Saved_bPrevWantsToCrouch) Result |= FLAG_PrevCrouch;
    return Result;
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
    FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
    UZeroBaseCharacterMovementComp* CharMovementComp = Cast<UZeroBaseCharacterMovementComp>(C->GetCharacterMovement());
    
    Saved_bWantsToSprint = CharMovementComp->Safe_bWantsToSprint;
    Saved_bWantsToDash = CharMovementComp->Safe_bWantsToDash;
    Saved_bPrevPressedJump = CharMovementComp->Safe_bPrevPressedJump;
    Saved_bPrevWantsToCrouch = CharMovementComp->Safe_bPrevWantsToCrouch;
    Saved_bHadAnimRootMotion = CharMovementComp->Safe_bHadAnimRootMotion;
    Saved_bTransitionFinished = CharMovementComp->Safe_bTransitionFinished;

    // Save state for client prediction rewind
    Saved_DashDir = CharMovementComp->Safe_DashDir;
    Saved_DashSpeed = CharMovementComp->Safe_DashSpeed;
    Saved_DashTimeRemaining = CharMovementComp->Safe_DashTimeRemaining;
    Saved_DashExitMode = CharMovementComp->Safe_DashExitMode;
    Saved_LeapTargetLoc = CharMovementComp->Safe_LeapTargetLoc;
    Saved_LeapTimeRemaining = CharMovementComp->Safe_LeapTimeRemaining;
}

void UZeroBaseCharacterMovementComp::FSavedMove_Zero::PrepMoveFor(ACharacter* C)
{
    FSavedMove_Character::PrepMoveFor(C);
    UZeroBaseCharacterMovementComp* CharMovementComp = Cast<UZeroBaseCharacterMovementComp>(C->GetCharacterMovement());
    
    CharMovementComp->Safe_bWantsToSprint = Saved_bWantsToSprint;
    CharMovementComp->Safe_bWantsToDash = Saved_bWantsToDash;
    CharMovementComp->Safe_bPrevPressedJump = Saved_bPrevPressedJump;
    CharMovementComp->Safe_bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;
    CharMovementComp->Safe_bHadAnimRootMotion = Saved_bHadAnimRootMotion;
    CharMovementComp->Safe_bTransitionFinished = Saved_bTransitionFinished;

    // Restore state if server corrects client
    CharMovementComp->Safe_DashDir = Saved_DashDir;
    CharMovementComp->Safe_DashSpeed = Saved_DashSpeed;
    CharMovementComp->Safe_DashTimeRemaining = Saved_DashTimeRemaining;
    CharMovementComp->Safe_DashExitMode = (EMovementMode)Saved_DashExitMode;
    CharMovementComp->Safe_LeapTargetLoc = Saved_LeapTargetLoc;
    CharMovementComp->Safe_LeapTimeRemaining = Saved_LeapTimeRemaining;
}

bool UZeroBaseCharacterMovementComp::FSavedMove_Zero::IsImportantMove(const FSavedMovePtr& LastAckedMove) const
{
    FSavedMove_Zero* LastAckedZeroMove = static_cast<FSavedMove_Zero*>(LastAckedMove.Get());
    if (LastAckedZeroMove)
    {
       if (bPressedJump && !LastAckedMove->bPressedJump) return true;
       if (Saved_bWantsToDash && !LastAckedZeroMove->Saved_bWantsToDash) return true;
    }
    return Super::IsImportantMove(LastAckedMove);
}
#pragma endregion

#pragma region PredictionData
UZeroBaseCharacterMovementComp::FNetworkPredictionData_Client_Zero::FNetworkPredictionData_Client_Zero(const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement) {}

FSavedMovePtr UZeroBaseCharacterMovementComp::FNetworkPredictionData_Client_Zero::AllocateNewMove()
{
    return FSavedMovePtr(new FSavedMove_Zero());
}
#pragma endregion

#pragma region CharMoveOverrides
UZeroBaseCharacterMovementComp::UZeroBaseCharacterMovementComp()
{
    Sprint_MaxSpeed = 1200;
    Walk_MaxSpeed = 600;
    NavAgentProps.bCanCrouch = true;

    SlideFriction = 0.75f;
    SlideMinSpeed = 250;
    SlideEnterImpulse = 500;
    SlideGravityForce = 5000;

    DashImpulse = 1750.0f;
    DashCoolDownDuration = 1.f;
    AuthDashCoolDownDuration = 0.9f;

    WallBounceImpluse = 1000.0f;

    MantleMaxDistance = 200.0f;
    MantleReachHeight = 50.0f;
    MinMantleDepth = 30.0f;
    MantleMinWallSteepnessAngle = 75.0f;
    MantleMaxSurfaceAngle = 40.0f;
    MantleMaxAlignmentAngle = 45.0f;

    ZiplineMinKeyPressTime = 0.15f;
    ZiplineCheckTickIntervel = 0.15f;
    ZiplineCheckSphereRadius = 110.0f;
    ZiplineCheckMaxDistance = 2000.0f;
    ZiplineSpeed = 1000.0f;

    QuickFallImpulse = 500.0f;
    bUseSeparateBrakingFriction = true;
}

void UZeroBaseCharacterMovementComp::InitializeComponent()
{
    Super::InitializeComponent();
    ZeroCharacter_Owner = Cast<AZeroLockCharacter>(GetOwner());
}

void UZeroBaseCharacterMovementComp::UpdateFromCompressedFlags(uint8 Flags)
{
    Super::UpdateFromCompressedFlags(Flags);
    Safe_bWantsToSprint = (Flags & FSavedMove_Zero::FLAG_Sprint) != 0;
    Safe_bWantsToDash = (Flags & FSavedMove_Zero::FLAG_Dash) != 0;
    
    Safe_bPrevPressedJump = (Flags & FSavedMove_Zero::FLAG_PrevJump) != 0;
    Safe_bPrevWantsToCrouch = (Flags & FSavedMove_Zero::FLAG_PrevCrouch) != 0;
}

void UZeroBaseCharacterMovementComp::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
    Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

    if(MovementMode == MOVE_Walking)
    {
       MaxWalkSpeed = Safe_bWantsToSprint ? Sprint_MaxSpeed : Walk_MaxSpeed;
    }
    
    Safe_bPrevWantsToCrouch = bWantsToCrouch;
    Safe_bPrevPressedJump = CharacterOwner->bPressedJump;
    
#if WITH_EDITOR
    if (GetWorld()->GetNetMode() == NM_Client || GetWorld()->GetNetMode() == NM_ListenServer || GetWorld()->GetNetMode() == NM_DedicatedServer)
    {
       int32 PlayerIndex = GetStablePlayerIndex();
       if (PlayerIndex >= 0 && PlayerIndex < 4)
       {
          if (IsServer())
          {
             GServerCharacterLoc[PlayerIndex] = UpdatedComponent->GetComponentLocation();
          }
          else if (CharacterOwner->IsLocallyControlled())
          {
             FVector ServerLoc = GServerCharacterLoc[PlayerIndex];
             if (!ServerLoc.IsZero())
             {
                 DrawDebugCapsule(GetWorld(), ServerLoc, CapHH(), CapR(), FQuat::Identity, FColor::Blue, false, -1.0f, 0, 2.0f);
             }
          }
       }
    }
#endif
}

void UZeroBaseCharacterMovementComp::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
    if (CharacterOwner->bPressedJump)
    {
        Safe_ZeroJumpHoldTime += DeltaSeconds;
    }
    else
    {
        Safe_ZeroJumpHoldTime = 0.0f;
    }

    if(Safe_ZeroJumpHoldTime > ZiplineMinKeyPressTime && CharacterOwner->bPressedJump)
    {
        if(TryZipLine())
        {
           SetMovementMode(MOVE_Custom, CMOVE_Zipline);
           return;
        }
    }
    
    if(CharacterOwner->bPressedJump && !Safe_bPrevPressedJump)
    {
       if(TryMantle())
       {
          CharacterOwner->StopJumping();
       }
       else if(TryWallBounce())
       {
          CharacterOwner->StopJumping();
          Proxy_bWallBounce = !Proxy_bWallBounce;
       }
    }
    
    bool bAuthProxy = IsServer() && !CharacterOwner->IsLocallyControlled();
    if(Safe_bWantsToDash && CanDash())
    {
       if(!bAuthProxy || GetWorld()->GetTimeSeconds() - DashStartTime > AuthDashCoolDownDuration)
       {
          PerformDash();
          Safe_bWantsToDash = false;
          Proxy_bDashStart = !Proxy_bDashStart;
       }
    }
    
    if(MovementMode == MOVE_Walking && bWantsToCrouch && Safe_bPrevWantsToCrouch)
    {
       float RequiredSpeed = CharacterOwner->IsLocallyControlled() ? SlideMinSpeed : (SlideMinSpeed - 50.f);
       FHitResult PotenialSurface;
       
       if(Velocity.SizeSquared() > pow(RequiredSpeed, 2) && GetSlideSurface(PotenialSurface))
       {
          SetMovementMode(MOVE_Custom, CMOVE_Slide);
          return;
       }
    }
    
    if(IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
    {
       SetMovementMode(MOVE_Walking);
    }
    
    Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UZeroBaseCharacterMovementComp::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
    Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
    
    if (TransitionRMS_ID > 0) 
    {
       TSharedPtr<FRootMotionSource> RMS = GetRootMotionSourceByID(TransitionRMS_ID);
       if (RMS && RMS->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
       {
          TSharedPtr<FRootMotionSource_MoveToForce> MoveToForceRMS = StaticCastSharedPtr<FRootMotionSource_MoveToForce>(RMS);
          if (MoveToForceRMS.IsValid())
          {
             FHitResult SnapHit;
             SafeMoveUpdatedComponent(MoveToForceRMS->TargetLocation - UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentQuat(), false, SnapHit);
          }

          Velocity = FVector::ZeroVector;
          SetMovementMode(MOVE_Walking);
          RemoveRootMotionSourceByID(TransitionRMS_ID);
           
          TransitionRMS_ID = 0; 
          Safe_bTransitionFinished = true;
       }
    }
    
    Safe_bHadAnimRootMotion = HasAnimRootMotion();
}

void UZeroBaseCharacterMovementComp::PhysCustom(float deltaTime, int32 Iterations)
{
    Super::PhysCustom(deltaTime, Iterations);
    switch (CustomMovementMode)
    {
    case CMOVE_Slide: PhysSlide(deltaTime, Iterations); break;
    case CMOVE_Zipline: PhysZipline(deltaTime, Iterations); break;
    case CMOVE_Melee: PhysMelee(deltaTime, Iterations); break;
    case CMOVE_DirectionalDash: PhysDirectionalDash(deltaTime, Iterations); break;
    case CMOVE_TargetedLeap: PhysTargetedLeap(deltaTime, Iterations); break;
    default: UE_LOG(LogTemp, Fatal, TEXT("InvalidMovement MOde"));
    }
}

void UZeroBaseCharacterMovementComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
    
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide) ExitSlide();
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Zipline) ExitZipline();
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Melee) ZLOG("EndMelee");
    
    if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_DirectionalDash) ExitDirectionalDash();

    if (IsCustomMovementMode(CMOVE_Slide)) EnterSlide();
    if (IsCustomMovementMode(CMOVE_Zipline)) EnterZipline();
    if (IsCustomMovementMode(CMOVE_Melee)) ZLOG("MeleeStart");

    if (ZeroMovementModeChangedDelegate.IsBound())
    {
       ZeroMovementModeChangedDelegate.Broadcast();
    }
}

bool UZeroBaseCharacterMovementComp::IsMovingOnGround() const
{
    return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UZeroBaseCharacterMovementComp::CanCrouchInCurrentState() const
{
    if (!CanEverCrouch()) return false;
    return (IsFalling() || IsMovingOnGround()) && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}
#pragma endregion 

#pragma region Directional Dash & Targeted Leap

// --- DEDICATED PHYSICS RPCS ---
void UZeroBaseCharacterMovementComp::StartDirectionalDash(const FVector& Direction, float Speed, float Duration, EMovementMode ExitMode)
{
    Safe_DashDir = Direction.GetSafeNormal();
    Safe_DashSpeed = Speed;
    Safe_DashTimeRemaining = Duration;
    Safe_DashExitMode = ExitMode;

    if (CharacterOwner->IsLocallyControlled() && !IsServer())
    {
        Server_StartDirectionalDash(Direction, Speed, Duration, ExitMode);
    }
    
    SetMovementMode(MOVE_Custom, CMOVE_DirectionalDash);
    if (OnDirectionalDashStarted.IsBound()) OnDirectionalDashStarted.Broadcast();
}

void UZeroBaseCharacterMovementComp::Server_StartDirectionalDash_Implementation(FVector Direction, float Speed, float Duration, EMovementMode ExitMode)
{
    StartDirectionalDash(Direction, Speed, Duration, ExitMode);
}

void UZeroBaseCharacterMovementComp::StartTargetedLeap(const FVector& TargetLocation, float Duration)
{
    Safe_LeapTargetLoc = TargetLocation;
    Safe_LeapTimeRemaining = Duration;

    if (CharacterOwner->IsLocallyControlled() && !IsServer())
    {
        Server_StartTargetedLeap(TargetLocation, Duration);
    }
    
    SetMovementMode(MOVE_Custom, CMOVE_TargetedLeap);
    if (OnTargetedLeapStarted.IsBound()) OnTargetedLeapStarted.Broadcast();
}

void UZeroBaseCharacterMovementComp::Server_StartTargetedLeap_Implementation(FVector TargetLocation, float Duration)
{
    StartTargetedLeap(TargetLocation, Duration);
}

void UZeroBaseCharacterMovementComp::PhysDirectionalDash(float DeltaTime, int32 Iterations)
{
    if (DeltaTime < MIN_TICK_TIME) return;

    // Sub-Frame Overshoot Fix
    float MoveDeltaTime = (Safe_DashTimeRemaining < DeltaTime) ? Safe_DashTimeRemaining : DeltaTime;

    Velocity = Safe_DashDir * Safe_DashSpeed;
    
    FHitResult Hit;
    SafeMoveUpdatedComponent(Velocity * MoveDeltaTime, UpdatedComponent->GetComponentQuat(), true, Hit);
    
    if (Hit.IsValidBlockingHit())
    {
        if (Hit.ImpactNormal.Z >= 0.5f)
        {
            SlideAlongSurface(Velocity * MoveDeltaTime, (1.f - Hit.Time), Hit.Normal, Hit, true);
        }
        else 
        {
            Safe_DashTimeRemaining = 0.0f;
            Velocity = FVector::ZeroVector;
            Acceleration = FVector::ZeroVector;
            
            SetMovementMode(Safe_DashExitMode);
            StartNewPhysics(DeltaTime, Iterations);
            
            if (OnDirectionalDashFinished.IsBound())
            {
                OnDirectionalDashFinished.Broadcast();
                OnDirectionalDashFinished.Clear(); 
            }
            return; 
        }
    }

    Safe_DashTimeRemaining -= DeltaTime;

    // Clean Finish
    if (Safe_DashTimeRemaining <= 0.0f)
    {
        Velocity = FVector::ZeroVector;
        Acceleration = FVector::ZeroVector; 
        
        SetMovementMode(Safe_DashExitMode);
        
        float LeftoverTime = DeltaTime - MoveDeltaTime;
        if (LeftoverTime > 0.0f)
        {
            StartNewPhysics(LeftoverTime, Iterations);
        }
        
        if (OnDirectionalDashFinished.IsBound())
        {
            OnDirectionalDashFinished.Broadcast();
            OnDirectionalDashFinished.Clear(); 
        }
    }
}

void UZeroBaseCharacterMovementComp::PhysTargetedLeap(float DeltaTime, int32 Iterations)
{
    if (DeltaTime < MIN_TICK_TIME) return;

    Safe_LeapTimeRemaining -= DeltaTime;

    if (Safe_LeapTimeRemaining <= 0.0f)
    {
       FVector RemainingDelta = Safe_LeapTargetLoc - UpdatedComponent->GetComponentLocation();
       FHitResult SnapHit;
        
       SafeMoveUpdatedComponent(RemainingDelta, UpdatedComponent->GetComponentQuat(), false, SnapHit);

       Velocity = FVector::ZeroVector;
       Acceleration = FVector::ZeroVector; 
        
       SetMovementMode(MOVE_Falling); 
       StartNewPhysics(DeltaTime, Iterations); 
        
       if (OnTargetedLeapFinished.IsBound())
       {
          OnTargetedLeapFinished.Broadcast();
          OnTargetedLeapFinished.Clear(); 
       }
       return;
    }

    FVector StartLoc = UpdatedComponent->GetComponentLocation();
    Velocity = (Safe_LeapTargetLoc - StartLoc) / Safe_LeapTimeRemaining;
    
    FHitResult Hit;
    SafeMoveUpdatedComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentQuat(), true, Hit);

    if (Hit.IsValidBlockingHit())
    {
       if (Hit.ImpactNormal.Z >= 0.5f)
       {
          SlideAlongSurface(Velocity * DeltaTime, (1.f - Hit.Time), Hit.Normal, Hit, true);
       }
       else 
       {
          Safe_LeapTimeRemaining = 0.0f;
          Velocity = FVector::ZeroVector;
          Acceleration = FVector::ZeroVector; 
            
          SetMovementMode(MOVE_Falling);
          StartNewPhysics(DeltaTime, Iterations);
            
          if (OnTargetedLeapFinished.IsBound())
          {
             OnTargetedLeapFinished.Broadcast();
             OnTargetedLeapFinished.Clear(); 
          }
       }
    }
}

void UZeroBaseCharacterMovementComp::ExitDirectionalDash()
{
    Velocity.X = 0.0f;
    Velocity.Y = 0.0f;
    Acceleration = FVector::ZeroVector;
    Safe_DashTimeRemaining = 0.0f;
}
#pragma endregion

#pragma region Slide
void UZeroBaseCharacterMovementComp::EnterSlide()
{
    Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
}

void UZeroBaseCharacterMovementComp::ExitSlide()
{
    FQuat NewRot = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
    float ZOffset = CapHH() - CapR(); 
    FHitResult Hit;
    
    SafeMoveUpdatedComponent(FVector(0, 0, ZOffset), NewRot, true, Hit);
    bJustTeleported = true;
}

void UZeroBaseCharacterMovementComp::PhysSlide(float DeltaTime, int32 Iterations)
{
    if(DeltaTime < MIN_TICK_TIME) return;

    RestorePreAdditiveRootMotionVelocity();

    float ExitSpeed = CharacterOwner->IsLocallyControlled() ? SlideMinSpeed : (SlideMinSpeed - 50.f);

    FHitResult SurfaceHit;
    if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(ExitSpeed, 2))
    {
       SetMovementMode(MOVE_Walking);
       StartNewPhysics(DeltaTime, Iterations);
       return;
    }

    Velocity += SlideGravityForce * FVector::DownVector * DeltaTime;

    if(FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
    {
       Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
    }
    else
    {
       Acceleration = FVector::ZeroVector;
    }

    if(!HasRootMotionSources() && !CurrentRootMotion.HasOverrideVelocity())
    {
       CalcVelocity(DeltaTime, SlideFriction, true, GetMaxBrakingDeceleration());
    }
    ApplyRootMotionToVelocity(DeltaTime);

    Iterations++;
    bJustTeleported = false;

    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    FHitResult Hit(1.f);
    FVector Adjusted = Velocity * DeltaTime;
    FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
    FQuat NewRot = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
    SafeMoveUpdatedComponent(Adjusted, NewRot, true, Hit);

    if(Hit.Time < 1.f)
    {
       HandleImpact(Hit, DeltaTime, Adjusted);
       SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
    }

    FHitResult NewSurfaceHit;
    if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(ExitSpeed, 2))
    {
       SetMovementMode(MOVE_Walking);
    }
    
    if(!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
       Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
    }
}

bool UZeroBaseCharacterMovementComp::GetSlideSurface(FHitResult& OutHit) const
{
    FVector Start = UpdatedComponent->GetComponentLocation();
    FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f * FVector::DownVector;
    FName ProfileName = TEXT("BlockAllDynamic");
    return GetWorld()->LineTraceSingleByProfile(OutHit, Start, End, ProfileName, ZeroCharacter_Owner->GetIgnoreCharacterParams());
}
#pragma endregion

#pragma region Dash
void UZeroBaseCharacterMovementComp::OnDashCoolDownFinished()
{
    Safe_bWantsToDash = true;
}

bool UZeroBaseCharacterMovementComp::CanDash() const { return true; }

void UZeroBaseCharacterMovementComp::PerformDash()
{
    DashStartTime = GetWorld()->GetTimeSeconds();
    FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration).GetSafeNormal2D();
    Velocity = DashImpulse * (DashDirection + FVector::UpVector * 0.25f);

    FQuat NewRot = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
    FHitResult Hit;
    SafeMoveUpdatedComponent(FVector::ZeroVector, NewRot, false, Hit);

    SetMovementMode(MOVE_Falling);

    if(DashStartDelegate.IsBound())
    {
       DashStartDelegate.Broadcast();
    }
}

void UZeroBaseCharacterMovementComp::OnRep_DashStart()
{
    if(Proxy_bDashStart && DashStartDelegate.IsBound())
    {
       DashStartDelegate.Broadcast();
    }
}
#pragma endregion

#pragma region Mantle
void UZeroBaseCharacterMovementComp::OnRep_ShortMantle() {}
void UZeroBaseCharacterMovementComp::OnRep_TallMantle() {}

void UZeroBaseCharacterMovementComp::OnRep_WallBounce()
{
    if (Proxy_bWallBounce && WallBounceDelegate.IsBound())
    {
       WallBounceDelegate.Broadcast();
    }
}

bool UZeroBaseCharacterMovementComp::TryMantle()
{
    if (!(IsMovementMode(MOVE_Walking) && !IsCrouching()) && !IsMovementMode(MOVE_Falling)) return false;
    
    if (Acceleration.IsNearlyZero() || (Acceleration.GetSafeNormal2D() | UpdatedComponent->GetForwardVector().GetSafeNormal2D()) < 0.5f) 
    {
       return false;
    }

    FVector BaseLoc = UpdatedComponent->GetComponentLocation() + FVector::DownVector * CapHH();
    FVector Fwd = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
    auto Params = ZeroCharacter_Owner->GetIgnoreCharacterParams();
    float MaxHeight = CapHH() * 2 + MantleReachHeight;
    float CosMMWSA = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));
    float CosMMSA = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
    float CosMMAA = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));

    FHitResult FrontHit;
    

    float CheckDistance = FMath::Clamp(Velocity | Fwd, CapR() + 30.f, MantleMaxDistance);
    

    if (IsServer() && !CharacterOwner->IsLocallyControlled())
    {
       CheckDistance += 100.0f; 
    }
    
    FVector FrontStart = BaseLoc + FVector::UpVector * (MaxStepHeight - 1);
    
    for (int i = 0; i < 6; i++)
    {
       if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + Fwd * CheckDistance, "BlockAll", Params)) break;
       FrontStart += FVector::UpVector * (2.f * CapHH() - (MaxStepHeight - 1)) / 5;
    }
    if (!FrontHit.IsValidBlockingHit()) return false;
    
    float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
    if (FMath::Abs(CosWallSteepnessAngle) > CosMMWSA || (Fwd | -FrontHit.Normal) < CosMMAA) return false;

    TArray<FHitResult> HeightHits;
    FHitResult SurfaceHit;
    FVector WallUp = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();
    float WallCos = FVector::UpVector | FrontHit.Normal;
    float WallSin = FMath::Sqrt(1 - WallCos * WallCos);
    FVector TraceStart = FrontHit.Location + Fwd + WallUp * (MaxHeight - (MaxStepHeight - 1)) / WallSin;
    
    if (!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + Fwd, "BlockAll", Params)) return false;
    for (const FHitResult& Hit : HeightHits)
    {
       if (Hit.IsValidBlockingHit())
       {
          SurfaceHit = Hit;
          break;
       }
    }
    if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMMSA) return false;
    float Height = (SurfaceHit.Location - BaseLoc) | FVector::UpVector;
    if (Height > MaxHeight) return false;

    float SurfaceCos = FVector::UpVector | SurfaceHit.Normal;
    float SurfaceSin = FMath::Sqrt(1 - SurfaceCos * SurfaceCos);
    FVector ClearCapLoc = SurfaceHit.Location + Fwd * CapR() + FVector::UpVector * (CapHH() + 1 + CapR() * 2 * SurfaceSin);
    FCollisionShape CapShape = FCollisionShape::MakeCapsule(CapR(), CapHH());
    
    if (GetWorld()->OverlapAnyTestByProfile(ClearCapLoc, FQuat::Identity, "BlockAll", CapShape, Params)) return false;

    FVector TransitionTarget = ClearCapLoc;
    float UpSpeed = Velocity | FVector::UpVector;
    float TransDistance = FVector::Dist(TransitionTarget, UpdatedComponent->GetComponentLocation());

    TransitionQueuedMontageSpeed = FMath::GetMappedRangeValueClamped(FVector2D(-500, 750), FVector2D(.9f, 1.2f), UpSpeed);
    TransitionRMS.Reset();
    TransitionRMS = MakeShared<FRootMotionSource_MoveToForce>();
    TransitionRMS->AccumulateMode = ERootMotionAccumulateMode::Override;
    TransitionRMS->Duration = FMath::Clamp(TransDistance / 500.f, .1f, .25f);
    TransitionRMS->StartLocation = UpdatedComponent->GetComponentLocation();
    TransitionRMS->TargetLocation = TransitionTarget;
    
    Velocity = FVector::ZeroVector;
    SetMovementMode(MOVE_Flying);
    TransitionRMS_ID = ApplyRootMotionSource(TransitionRMS);
    TransitionName = "Mantle";
    return true;
}
#pragma endregion

#pragma region WallBounce
bool UZeroBaseCharacterMovementComp::TryWallBounce()
{
    if (!IsMovementMode(MOVE_Falling)) return false;

    FHitResult GroundHit;
    FVector StartLoc = UpdatedComponent->GetComponentLocation();
    FVector EndLoc = StartLoc + FVector::DownVector * (CapHH() + 30.0f); 
    
    if (GetWorld()->LineTraceSingleByChannel(GroundHit, StartLoc, EndLoc, ECC_WorldStatic, ZeroCharacter_Owner->GetIgnoreCharacterParams()))
    {
        return false;
    }

    FHitResult WallHit;
    FCollisionShape CapShape = FCollisionShape::MakeCapsule(CapR() * 1.25f, CapHH() / 2);
    
    FVector TraceStart = UpdatedComponent->GetComponentLocation();
    FVector TraceEnd = TraceStart;

    if (IsServer() && !CharacterOwner->IsLocallyControlled())
    {
        FVector PredictDir = Velocity.GetSafeNormal2D();
        if (PredictDir.IsNearlyZero()) PredictDir = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
        TraceEnd += PredictDir * 80.0f; 
    }

    if(GetWorld()->SweepSingleByChannel(WallHit, TraceStart, TraceEnd, FQuat::Identity, ECC_WorldStatic, CapShape, ZeroCharacter_Owner->GetIgnoreCharacterParams()))
    {
       if (FMath::Abs(WallHit.ImpactNormal.Z) > 0.3f) return false;

       FVector CurrentVelocityXY = Velocity;
       CurrentVelocityXY.Z = 0.0f;
       
       FVector ParallelVelocity = FVector::VectorPlaneProject(CurrentVelocityXY, WallHit.ImpactNormal);
       FVector WallLaunchDir = (WallHit.ImpactNormal.GetSafeNormal() + FVector::UpVector).GetSafeNormal();
       
       Velocity = ParallelVelocity + (WallLaunchDir * WallBounceImpluse);
       
       if (WallBounceDelegate.IsBound()) WallBounceDelegate.Broadcast();
       SetMovementMode(MOVE_Falling);
       return true;
    }
    return false;
}
#pragma endregion

#pragma region Zipline
bool UZeroBaseCharacterMovementComp::TryZipLine()
{
    
    if(IsCustomMovementMode(CMOVE_Zipline)) return false;
    if(Safe_ZeroJumpHoldTime < ZiplineMinKeyPressTime) return false;
    
    FCollisionShape ZipCap = FCollisionShape::MakeSphere(ZiplineCheckSphereRadius);
    
    FVector SyncedAimDir = CharacterOwner->GetBaseAimRotation().Vector();
    FVector TraceLocation = UpdatedComponent->GetComponentLocation() + (SyncedAimDir * ZiplineCheckSphereRadius);
    FVector TraceEndLocation = UpdatedComponent->GetComponentLocation() + (SyncedAimDir * ZiplineCheckMaxDistance);
    
    FHitResult ZipHit;
    
    if(GetWorld()->SweepSingleByObjectType(ZipHit, TraceLocation, TraceEndLocation, SyncedAimDir.ToOrientationQuat(), ECC_Vehicle, ZipCap, ZeroCharacter_Owner->GetIgnoreCharacterParams()))
    {
       if(Cast<AZero_ZiplineActor>(ZipHit.GetActor()))
       {
          ZiplineActorRef = Cast<AZero_ZiplineActor>(ZipHit.GetActor());
          ZiplineSplineComp = ZiplineActorRef->GetZiplineComponent();
          float ClosestDistancetoPlayer = ZiplineSplineComp->GetDistanceAlongSplineAtLocation(CharLocation(), ESplineCoordinateSpace::World);
          float maxDis = ZiplineSplineComp->GetSplineLength();
          FVector ZiplineDir = ZiplineSplineComp->GetDirectionAtDistanceAlongSpline(ClosestDistancetoPlayer, ESplineCoordinateSpace::World);
          float Angle = CharacterOwner->GetActorForwardVector() | ZiplineDir;
          
          bZiplineMoveingToEnd = (Angle > 0);
          return true;
       }
    }
    return false;
}

void UZeroBaseCharacterMovementComp::EnterZipline()
{
    Velocity = FVector::ZeroVector;
}

void UZeroBaseCharacterMovementComp::ExitZipline() {}

void UZeroBaseCharacterMovementComp::PhysZipline(float DeltaTime, int32 Iterations)
{
    if(DeltaTime < MIN_TICK_TIME) return;

    RestorePreAdditiveRootMotionVelocity();
    if(!ZiplineSplineComp) 
    {
       FHitResult ProxyHit;
       SafeMoveUpdatedComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentQuat(), false, ProxyHit);
       return;
    }
    
    if(Safe_bWantsToDash || bWantsToCrouch)
    {
       SetMovementMode(MOVE_Falling);
       StartNewPhysics(DeltaTime, Iterations);
       return;
    }

    Iterations++;
    bJustTeleported = false;
    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    float MaxDis = ZiplineSplineComp->GetSplineLength();
    float DistancetoPoint = ZiplineSplineComp->GetDistanceAlongSplineAtLocation(CharLocation(), ESplineCoordinateSpace::World);
    float DistanceToMoveAlongSpline = bZiplineMoveingToEnd ? ZiplineSpeed : (ZiplineSpeed * -1);
    FVector TargetLocation = ZiplineSplineComp->GetLocationAtDistanceAlongSpline(DistancetoPoint + DistanceToMoveAlongSpline, ESplineCoordinateSpace::World);
    FVector Adjusted = (TargetLocation - CharLocation()).GetSafeNormal() * DeltaTime * ZiplineSpeed;
    
    if((bZiplineMoveingToEnd && MaxDis <= DistancetoPoint) || (!bZiplineMoveingToEnd && DistancetoPoint <= 0.0f))
    {
       SetMovementMode(MOVE_Falling);
       StartNewPhysics(DeltaTime, Iterations);
       return;
    }

    FHitResult Hit(1.f);
    FVector FVofSplinePoint = ZiplineSplineComp->GetDirectionAtDistanceAlongSpline(DistancetoPoint, ESplineCoordinateSpace::World).GetSafeNormal();
    FVofSplinePoint.Z = 0;
    if(!bZiplineMoveingToEnd) FVofSplinePoint *= -1;
    
    FQuat NewRot = FRotationMatrix::MakeFromXZ(FVofSplinePoint, FVector::UpVector).ToQuat();
    SafeMoveUpdatedComponent(Adjusted, NewRot, true, Hit);

    if(!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
       Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
    }
}

void UZeroBaseCharacterMovementComp::QuickFallDown() {}
#pragma endregion

#pragma region Helper Functions
bool UZeroBaseCharacterMovementComp::IsServer() const { return CharacterOwner->HasAuthority(); }
float UZeroBaseCharacterMovementComp::CapR() const { return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(); }
float UZeroBaseCharacterMovementComp::CapHH() const { return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(); }
bool UZeroBaseCharacterMovementComp::IsMovementMode(EMovementMode inMovementMode) const { return inMovementMode == MovementMode; }
FVector UZeroBaseCharacterMovementComp::CharLocation() const { return UpdatedComponent->GetComponentLocation(); }
FRotator UZeroBaseCharacterMovementComp::CharRotation() const { return UpdatedComponent->GetComponentRotation(); }

int32 UZeroBaseCharacterMovementComp::GetStablePlayerIndex() const
{
    AGameStateBase* GS = GetWorld()->GetGameState();
    if (GS && !GS->PlayerArray.IsEmpty() && CharacterOwner)
    {
       TArray<APlayerState*> SortedArray = GS->PlayerArray;
       SortedArray.Sort([](const APlayerState& A, const APlayerState& B) {
          return A.GetPlayerId() < B.GetPlayerId();
       });
       return SortedArray.IndexOfByKey(CharacterOwner->GetPlayerState());
    }
    return -1;
}

void UZeroBaseCharacterMovementComp::OnClientCorrectionReceived(
    class FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation,
    FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition,
    uint8 ServerMovementMode, FVector ServerGravityDirection)
{
    Super::OnClientCorrectionReceived(ClientData, TimeStamp, NewLocation, NewVelocity, NewBase, NewBaseBoneName,
                                      bHasBase, bBaseRelativePosition,
                                      ServerMovementMode, ServerGravityDirection);
    
    const FVector OldLocation = UpdatedComponent->GetComponentLocation();
    UE_VLOG_SPHERE(this, LogZeroMovement, Error, OldLocation, 10.0f, FColor::Red, TEXT("Pre-correction"));
    UE_VLOG_ARROW(this, LogZeroMovement, Error, OldLocation, NewLocation, FColor::Yellow, TEXT("Correction Shift"));
    UE_VLOG_SPHERE(this, LogZeroMovement, Error, NewLocation, 10.0f, FColor::Green, TEXT("Post-correction"));
    UE_VLOG(this, LogZeroMovement, Error, TEXT("Correction! Client Mode: %d | Server Mode: %d"), MovementMode.GetIntValue(), ServerMovementMode);
}

bool UZeroBaseCharacterMovementComp::IsCustomMovementMode(ECustomMovementMode inCustomMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == inCustomMode; }

void UZeroBaseCharacterMovementComp::PhysMelee(float DeltaTime, int32 Iterations)
{
    if (DeltaTime < MIN_TICK_TIME) return;

    RestorePreAdditiveRootMotionVelocity();
    Iterations++;
    bJustTeleported = false;

    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    
    FVector MeleeDir = CharacterOwner->GetBaseAimRotation().Vector();
    MeleeDir.Z = 0.f; 
    MeleeDir.Normalize();

    FVector MoveDelta = MeleeDir * (1000.f * DeltaTime); 
    FQuat NewRot = FRotationMatrix::MakeFromXZ(MeleeDir, FVector::UpVector).ToQuat();
    
    FHitResult PlayerHit;
    FCollisionShape CapShape = CharacterOwner->GetCapsuleComponent()->GetCollisionShape();
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(CharacterOwner);

    bool bHitPlayer = GetWorld()->SweepSingleByObjectType(
        PlayerHit, 
        OldLocation, 
        OldLocation + MoveDelta, 
        CharacterOwner->GetActorQuat(), 
        FCollisionObjectQueryParams(ECC_GameTraceChannel1), 
        CapShape, 
        Params
    );

    if (bHitPlayer)
    {
        MoveDelta *= PlayerHit.Time;
    }

    FHitResult WallHit(1.f);
    SafeMoveUpdatedComponent(MoveDelta, NewRot, true, WallHit);

    if (WallHit.Time < 1.f)
    {
        SlideAlongSurface(MoveDelta, (1.f - WallHit.Time), WallHit.Normal, WallHit, true);
    }

    if (bHitPlayer)
    {
        if (MeleeHitDelegate.IsBound())
        {
            MeleeHitDelegate.Broadcast();
        }
        SetMovementMode(MOVE_Falling);
    }

    if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
        Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
    }
}
#pragma endregion

#pragma region Inputs
void UZeroBaseCharacterMovementComp::SprintPressed() { Safe_bWantsToSprint = true; }
void UZeroBaseCharacterMovementComp::SprintReleased() { Safe_bWantsToSprint = false; }

void UZeroBaseCharacterMovementComp::CrouchPressed() { bWantsToCrouch = true; }
void UZeroBaseCharacterMovementComp::CrouchReleased() { bWantsToCrouch = false; }

void UZeroBaseCharacterMovementComp::DashPressed()
{
    float currentTime = GetWorld()->GetTimeSeconds();
    if(currentTime - DashStartTime >= DashCoolDownDuration)
    {
       Safe_bWantsToDash = true;
    }
    else
    {
       GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashCoolDown, this, &ThisClass::OnDashCoolDownFinished, DashCoolDownDuration - (currentTime - DashStartTime));
    }
}

void UZeroBaseCharacterMovementComp::DashReleased()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DashCoolDown);
    Safe_bWantsToDash = false;
}

void UZeroBaseCharacterMovementComp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UZeroBaseCharacterMovementComp, Proxy_bDashStart, COND_SkipOwner);
    DOREPLIFETIME_CONDITION(UZeroBaseCharacterMovementComp, Proxy_bWallBounce, COND_SkipOwner);
}
#pragma endregion

class FNetworkPredictionData_Client* UZeroBaseCharacterMovementComp::GetPredictionData_Client() const
{
    check(PawnOwner != nullptr)
    if(ClientPredictionData == nullptr)
    {
       UZeroBaseCharacterMovementComp* MutableThis = const_cast<UZeroBaseCharacterMovementComp*>(this);
       MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Zero(*this);
       
       MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 256.f * 2;
       MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 384.f * 2;
    }
    return ClientPredictionData;
}