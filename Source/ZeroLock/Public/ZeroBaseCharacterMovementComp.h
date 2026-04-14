//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroBaseCharacterMovementComp.generated.h"

class USplineComponent;
class AZero_ZiplineActor;
class AZeroLockCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWallBounceDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeHitDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMMDelegate);

UENUM(BlueprintType)
enum ECustomMovementMode
{
    CMOVE_None      UMETA(DisplayName = "None"),
    CMOVE_Slide     UMETA(DisplayName = "Slide"),
    CMOVE_Zipline   UMETA(DisplayName = "Zipline"),
    CMOVE_Melee     UMETA(DisplayName = "Melee"),
    CMOVE_To_Location UMETA(DisplayName = "To_Location"),
    CMOVE_Max       UMETA(DisplayName = "Max")
};

UCLASS()
class ZEROLOCK_API UZeroBaseCharacterMovementComp : public UCharacterMovementComponent
{
    GENERATED_BODY()

public:

    class FSavedMove_Zero : public FSavedMove_Character
    {
    public:
        enum CompressedFlags
        {
            FLAG_Sprint      = 0x10,
            FLAG_Dash        = 0x20
            // Native bPressedJump is handled by Unreal Engine natively
        };
        typedef FSavedMove_Character Super;

        uint8 Saved_bWantsToSprint: 1;
        uint8 Saved_bWantsToDash: 1;
        uint8 Saved_bPrevPressedJump: 1; 
        uint8 Saved_bPrevWantsToCrouch: 1;
        uint8 Saved_bHadAnimRootMotion: 1;
        uint8 Saved_bTransitionFinished: 1;

    public:
        FSavedMove_Zero();

        virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
        virtual void Clear() override;
        virtual uint8 GetCompressedFlags() const override;
        virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
        virtual void PrepMoveFor(ACharacter* C) override;
        virtual bool IsImportantMove(const FSavedMovePtr& LastAckedMove) const override;
    };

    class FNetworkPredictionData_Client_Zero : public FNetworkPredictionData_Client_Character
    {
    public:
        FNetworkPredictionData_Client_Zero(const UCharacterMovementComponent& ClientMovement);
        typedef FNetworkPredictionData_Client_Character Super;
        virtual FSavedMovePtr AllocateNewMove() override;
    };

    // --- Component Safe Variables ---
    bool Safe_bWantsToSprint;
    bool Safe_bWantsToDash;
    bool Safe_bPrevPressedJump; 
    bool Safe_bPrevWantsToCrouch;
    bool Safe_bHadAnimRootMotion;
    bool Safe_bTransitionFinished;

    float Safe_ZeroJumpHoldTime;

    float DashStartTime;
    FTimerHandle TimerHandle_DashCoolDown;

    UPROPERTY(BlueprintAssignable)
    FMMDelegate ZeroMovementModeChangedDelegate;

    // Replicated Triggers
    UPROPERTY(ReplicatedUsing=OnRep_DashStart) bool Proxy_bDashStart;
    UPROPERTY(ReplicatedUsing=OnRep_ShortMantle) bool Proxy_bShortMantle;
    UPROPERTY(ReplicatedUsing=OnRep_TallMantle) bool Proxy_bTallMantle;
    UPROPERTY(ReplicatedUsing=OnRep_WallBounce) bool Proxy_bWallBounce;

    // Transient
    UPROPERTY(transient) AZeroLockCharacter* ZeroCharacter_Owner;

    TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
    FString TransitionName;
    UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
    float TransitionQueuedMontageSpeed;
    int TransitionRMS_ID;
    
public:
    UPROPERTY(EditDefaultsOnly) float Sprint_MaxSpeed;
    UPROPERTY(EditDefaultsOnly) float Walk_MaxSpeed;

    // Slide
    UPROPERTY(EditDefaultsOnly) float SlideMinSpeed;
    UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse;
    UPROPERTY(EditDefaultsOnly) float SlideGravityForce;
    UPROPERTY(EditDefaultsOnly) float SlideFriction;

    // Dash
    UPROPERTY(EditDefaultsOnly) float DashImpulse;
    UPROPERTY(EditDefaultsOnly) float DashCoolDownDuration;
    UPROPERTY(EditDefaultsOnly) float AuthDashCoolDownDuration;
    UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;

    // WallBounce
    UPROPERTY(EditDefaultsOnly) float WallBounceImpluse;
    UPROPERTY(BlueprintAssignable) FWallBounceDelegate WallBounceDelegate;

    // Mantle
    UPROPERTY(EditDefaultsOnly) float MantleMaxDistance;
    UPROPERTY(EditDefaultsOnly) float MantleReachHeight;
    UPROPERTY(EditDefaultsOnly) float MinMantleDepth;
    UPROPERTY(EditDefaultsOnly) float MantleMinWallSteepnessAngle;
    UPROPERTY(EditDefaultsOnly) float MantleMaxSurfaceAngle;
    UPROPERTY(EditDefaultsOnly) float MantleMaxAlignmentAngle;

    // Zipline
    UPROPERTY(EditDefaultsOnly) float ZiplineMinKeyPressTime;
    UPROPERTY(EditDefaultsOnly) float ZiplineCheckTickIntervel;
    UPROPERTY(EditDefaultsOnly) float ZiplineCheckSphereRadius;
    UPROPERTY(EditDefaultsOnly) float ZiplineCheckMaxDistance;
    UPROPERTY(EditDefaultsOnly) float ZiplineSpeed;
    float ZiplineLastTickTime;
    bool bZiplineMoveingToEnd;
    AZero_ZiplineActor* ZiplineActorRef;
    TObjectPtr<USplineComponent> ZiplineSplineComp;

    // DashJump / QuickFall
    UPROPERTY(EditDefaultsOnly) float DashJumpStartTime;
    UPROPERTY(EditDefaultsOnly) float DashJumpEndTime;
    UPROPERTY(EditDefaultsOnly) float QuickFallImpulse;
    bool bCanQuickFall;
    
    UPROPERTY(BlueprintAssignable)
    FMeleeHitDelegate MeleeHitDelegate;
    
    UZeroBaseCharacterMovementComp();

    virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
    virtual void InitializeComponent() override;
    virtual void UpdateFromCompressedFlags(uint8 Flags) override;
    virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
    virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
    virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
    virtual void PhysCustom(float deltaTime, int32 Iterations) override;
    virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
    
public:
    virtual bool IsMovingOnGround() const override;
    virtual bool CanCrouchInCurrentState() const override;
    
private:
    // Slide
    void EnterSlide();
    void ExitSlide();
    void PhysSlide(float DeltaTime, int32 Iterations);
    bool GetSlideSurface(FHitResult& OutHit) const;

    // Dash
    void OnDashCoolDownFinished();
    bool CanDash() const;
    void PerformDash();
    UFUNCTION() void OnRep_DashStart();

    UFUNCTION() void OnRep_ShortMantle();
    UFUNCTION() void OnRep_TallMantle();
    UFUNCTION() void OnRep_WallBounce();

    // Trace Abilities
    bool TryMantle();
    bool TryWallBounce();
    bool TryZipLine();
    void EnterZipline();
    void ExitZipline();
    void PhysZipline(float DeltaTime, int32 Iterations);

    // Misc
    void QuickFallDown();
    void PhysMelee(float DeltaTime, int32 Iterations);
    
    // Helpers
    bool IsServer() const;
    float CapR() const;
    float CapHH() const;
    bool IsMovementMode(EMovementMode inMovementMode) const;
    FVector CharLocation() const;
    FRotator CharRotation() const;
    
    // Debug
    int32 GetStablePlayerIndex() const;
    virtual void OnClientCorrectionReceived(class FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection) override;
    
public:
    UFUNCTION(BlueprintPure,BlueprintCallable) bool IsCustomMovementMode(ECustomMovementMode inCustomMode) const;
    
    // Network Inputs
    UFUNCTION(BlueprintCallable) void SprintPressed();
    UFUNCTION(BlueprintCallable) void SprintReleased();
    
    UFUNCTION(BlueprintCallable) void CrouchPressed();
    UFUNCTION(BlueprintCallable) void CrouchReleased();

    UFUNCTION(BlueprintCallable) void DashPressed();
    UFUNCTION(BlueprintCallable) void DashReleased();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};