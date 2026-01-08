//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroBaseCharacterMovementComp.generated.h"

/**
 * 
 */

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
	CMOVE_None  UMETA(DisplayName = "None"),
	CMOVE_Slide UMETA(DisplayName = "Slide"),
	CMOVE_Zipline UMETA(DisplayName = "Zipline"),
	CMOVE_Melee UMETA(DisplayName = "Melee"),
	CMOVE_Max   UMETA(DisplayName = "Max")
};

UCLASS()
class ZEROLOCK_API UZeroBaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Zero :public FSavedMove_Character
	{

		public:
		enum CompressedFlags
		{
			FLAG_Sprint  = 0x10,
			FLAG_Dash    = 0x20,
			FLAG_Cust3	 = 0x40,
			FLAG_Cust4   = 0x80,
			
		};
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint: 1;
		uint8 Saved_bWantsToDash: 1;
		uint8 Saved_bPressedZeroJump:1;

		uint8 Saved_bPrevWantsToCrouch: 1;
		uint8 Saved_bHadAnimRootMotion:1;
		uint8 Saved_bTransitionFinished:1;

	
		
	public:
		FSavedMove_Zero();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Zero :public FNetworkPredictionData_Client_Character
	{
		public:
		FNetworkPredictionData_Client_Zero(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	//Flags
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
	

	bool Safe_bWantsToDash;
	bool Safe_bHadAnimRootMotion;

	

	float DashStartTime;//time the dash starts
	FTimerHandle TimerHandle_DashCoolDown;


	UPROPERTY(BlueprintAssignable)
	FMMDelegate ZeroMovementModeChangedDelegate;

	//replicated
	UPROPERTY(ReplicatedUsing=OnRep_DashStart) bool Proxy_bDashStart;
	UPROPERTY(ReplicatedUsing=OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing=OnRep_TallMantle) bool Proxy_bTallMantle;
	UPROPERTY(ReplicatedUsing=OnRep_WallBounce) bool Proxy_bWallBounce;

	//transient
	UPROPERTY(transient) AZeroLockCharacter* ZeroCharacter_Owner;

	bool Safe_bTransitionFinished;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	FString TransitionName;
	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;
	
public:
	UPROPERTY(EditDefaultsOnly) float Sprint_MaxSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxSpeed;

	UPROPERTY(EditDefaultsOnly) float SlideMinSpeed;
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse;
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce;
	UPROPERTY(EditDefaultsOnly) float SlideFriction;

	//Dash
	UPROPERTY(EditDefaultsOnly) float DashImpulse;
	UPROPERTY(EditDefaultsOnly) float DashCoolDownDuration;
	UPROPERTY(EditDefaultsOnly) float AuthDashCoolDownDuration;
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;

	//WallBounce
	UPROPERTY(EditDefaultsOnly) float WallBounceImpluse;
	UPROPERTY(BlueprintAssignable) FWallBounceDelegate WallBounceDelegate;
	//Mantle
	UPROPERTY(EditDefaultsOnly) float MantleMaxDistance ;
	UPROPERTY(EditDefaultsOnly) float MantleReachHeight ;
	UPROPERTY(EditDefaultsOnly) float MinMantleDepth ;
	UPROPERTY(EditDefaultsOnly) float MantleMinWallSteepnessAngle ;
	UPROPERTY(EditDefaultsOnly) float MantleMaxSurfaceAngle ;
	UPROPERTY(EditDefaultsOnly) float MantleMaxAlignmentAngle;

	

	//Zipline
	UPROPERTY(EditDefaultsOnly) float ZiplineMinKeyPressTime ;
	UPROPERTY(EditDefaultsOnly) float ZiplineCheckTickIntervel;
	UPROPERTY(EditDefaultsOnly) float ZiplineCheckSphereRadius ;
	UPROPERTY(EditDefaultsOnly) float ZiplineCheckMaxDistance ;
	UPROPERTY(EditDefaultsOnly) float ZiplineSpeed;
	float ZiplineLastTickTime;
	bool bZiplineMoveingToEnd;
	AZero_ZiplineActor* ZiplineActorRef;
	TObjectPtr<USplineComponent> ZiplineSplineComp;

	//DashJump
	UPROPERTY(EditDefaultsOnly) float DashJumpStartTime;
	UPROPERTY(EditDefaultsOnly) float DashJumpEndTime;

	//QuickFall
	UPROPERTY(EditDefaultsOnly) float QuickFallImpulse;
	bool bCanQuickFall;
	
	UPROPERTY(BlueprintAssignable)
	FMeleeHitDelegate MeleeHitDelegate;
	
	UZeroBaseCharacterMovementComp();

	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void InitializeComponent() override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	//called after every perform move (kinda like Tick)
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	//virtual bool DoJump(bool bReplayingMoves) override;
	
public:
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	//virtual bool CanAttemptJump() const override;
	//Slide
	private:
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& OutHit) const;

	//Dash
	void OnDashCoolDownFinished();
	bool CanDash() const;
	void PerformDash();
	UFUNCTION()
	void OnRep_DashStart();

	

	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();

	UFUNCTION() void OnRep_WallBounce();

	//Mantle
	bool TryMantle();
	
	//Wall Bounce
	bool TryWallBounce();
	
	//Zipline
	bool TryZipLine();
	UFUNCTION(Server, Reliable) void Server_EnterZipline(USplineComponent* ZiplineToUse , bool InSplineDir);
	void EnterZipline();
	void ExitZipline();
	void PhysZipline(float DeltaTime, int32 Iterations);

	//QuickFall
	void QuickFallDown();
	
	//Melee
	void PhysMelee(float DeltaTime, int32 Iterations);

	

	bool IsServer() const;
	float CapR() const;
	float CapHH() const;
	bool IsMovementMode(EMovementMode inMovementMode)const;
	FVector CharLocation()const;
	FRotator CharRotation()const;
	FVector CamFV()const;
	FVector CamLoc()const;
	FQuat CamQuat()const;

	
	
public:
	UFUNCTION(BlueprintPure,BlueprintCallable) bool IsCustomMovementMode(ECustomMovementMode inCustomMode) const;
	
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();


	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	

};
