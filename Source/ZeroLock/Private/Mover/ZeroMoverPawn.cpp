// Copyright Preetham Mukundan (C) 2026

#include "Mover/ZeroMoverPawn.h"
#include "Mover/ZeroMovementData.h" 

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/LayeredMoves/BasicLayeredMoves.h"
#include "GameFramework/SpringArmComponent.h"
#include "DefaultMovementSet/LayeredMoves/MultiJumpLayeredMove.h"
#include "ZeroLock/ZeroLock.h"

AZeroMoverPawn::AZeroMoverPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicatingMovement(false);
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
    CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    CapsuleComponent->bDynamicObstacle = true;
    RootComponent = CapsuleComponent;

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    
    MeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
    
    MoverComponent = CreateDefaultSubobject<UCharacterMoverComponent>(TEXT("MoverComponent"));
    MoverComponent->SetIsReplicated(true);
 
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 400.0f; 
    SpringArmComponent->bUsePawnControlRotation = true; 

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false; 
}

void AZeroMoverPawn::OnMoverStanceChanged(EStanceMode OldStance, EStanceMode NewStance)
{
    
}

void AZeroMoverPawn::OnMovementModeChanged(const FName& PreviousMovementModeName, const FName& NewMovementModeName)
{
    
}

void AZeroMoverPawn::OnPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd)
{
    if (const FCharacterDefaultInputs* FoundDefaults = InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>())
    {
        DefaultInputData = *FoundDefaults;
    }

    if (const FZeroMovementInputs* FoundZeroInputs = InputCmd.InputCollection.FindDataByType<FZeroMovementInputs>())
    {
        ZeroInputsData = *FoundZeroInputs;
    }
    
    HandleDashInputs();
}

void AZeroMoverPawn::HandleDashInputs()
{
    if (ZeroInputsData.bWantsToDash)
    {
        TriggerDash(1500.0f, 0.2f);
    }
}

void AZeroMoverPawn::BeginPlay()
{
    Super::BeginPlay();
    
    MoverComponent->OnStanceChanged.AddUniqueDynamic(this,&AZeroMoverPawn::OnMoverStanceChanged);
    MoverComponent->OnMovementModeChanged.AddUniqueDynamic(this,&AZeroMoverPawn::OnMovementModeChanged);
    MoverComponent->OnPreSimulationTick.AddUniqueDynamic(this,&AZeroMoverPawn::OnPreSimulationTick);
}

void AZeroMoverPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (GEngine && MoverComponent)
    {
        FName CurrentMode = MoverComponent->GetMovementModeName();
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Cyan, FString::Printf(TEXT("Current Mode: %s"), *CurrentMode.ToString()));
    }
}

void AZeroMoverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnMove);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AZeroMoverPawn::OnMove);
        
        EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnJumpPressed);
        EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AZeroMoverPawn::OnJumpReleased);
        
        EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnCrouchPressed);
        EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Completed,this,&AZeroMoverPawn::OnCrouchReleased);
        
        EnhancedInputComponent->BindAction(DashAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnDashPressed);
   
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnLook);
    }
}

void AZeroMoverPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
    FCharacterDefaultInputs& DefaultInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();
    
    if (!DefaultInputs.bIsJumpJustPressed && bLocalJumpPressed)
    {
        DefaultInputs.bIsJumpJustPressed = true;
    }
    else
    {
        DefaultInputs.bIsJumpJustPressed = false;
    }
    DefaultInputs.bIsJumpPressed = bLocalJumpPressed;
   

    FVector ControlVector = FVector::ZeroVector;
    if (Controller && !CachedMoveInput.IsZero())
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        ControlVector = (ForwardDirection * CachedMoveInput.Y) + (RightDirection * CachedMoveInput.X);
        ControlVector.Normalize();
    }


    DefaultInputs.SetMoveInput(EMoveInputType::DirectionalIntent, ControlVector); 


    if (Controller)
    {
        DefaultInputs.ControlRotation = Controller->GetControlRotation();

        DefaultInputs.OrientationIntent = DefaultInputs.ControlRotation.Vector().GetSafeNormal();
    }
    else
    {
        DefaultInputs.OrientationIntent = GetActorForwardVector();
    }
    
    if (MoverComponent && MoverComponent->GetMovementModeName() == TEXT("Sliding"))
    {
        bLocalSlideIntentValid = false;
    }

    FName CurrentMode = MoverComponent->GetMovementModeName();

    if (CurrentMode == DefaultModeNames::Walking || CurrentMode == TEXT("Sliding") || CurrentMode == TEXT("WallJumping"))
    {
        LocalAirJumpsUsed = 0;
    }
    
    bool bCustomJump = (bLocalJumpPressed && !bWasJumpPressedLastFrame);
    bWasJumpPressedLastFrame = bLocalJumpPressed;
    
    bool bValidAirJump = false;
    if (bCustomJump && CurrentMode == DefaultModeNames::Falling && LocalAirJumpsUsed < SlideSettings->MaxAirJumps)
    {
        bValidAirJump = true;
        LocalAirJumpsUsed++;
    }
    
    FZeroMovementInputs& ZeroInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FZeroMovementInputs>();
    
    //ZeroInputs.MoveInput = ControlVector;
    ZeroInputs.bWantsToCrouch = bCachedWantsToCrouch;
    ZeroInputs.bSlideIntentValid = bLocalSlideIntentValid;
    ZeroInputs.bCustomJumpJustPressed = bCustomJump;
    ZeroInputs.bWantsToAirJump = bValidAirJump;
    ZeroInputs.bWantsToDash = bWantsToDashLatch;
    if (Controller)
    {
        ZeroInputs.LookDir = Controller->GetControlRotation();
    }
    
    bWantsToDashLatch = false;
}

void AZeroMoverPawn::OnMove(const FInputActionValue& Value)
{
    CachedMoveInput = Value.Get<FVector2D>();
}

void AZeroMoverPawn::OnLook(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void AZeroMoverPawn::OnJumpPressed()
{
    bLocalJumpPressed = true;
}

void AZeroMoverPawn::OnJumpReleased()
{
    bLocalJumpPressed = false;
}

void AZeroMoverPawn::OnCrouchPressed()
{
    bCachedWantsToCrouch = true;
    MoverComponent->Crouch();
}

void AZeroMoverPawn::OnCrouchReleased()
{
    bCachedWantsToCrouch = false;
    bLocalSlideIntentValid = true;
    MoverComponent->UnCrouch();
}

void AZeroMoverPawn::OnDashPressed()
{
    bWantsToDashLatch = true;
}

void AZeroMoverPawn::TriggerDash(float DashSpeed, float DurationSeconds)
{
    if (!MoverComponent) return;

    TSharedPtr<FLayeredMove_LinearVelocity> DashMove = MakeShared<FLayeredMove_LinearVelocity>();
    
    FVector MoveIntent = DefaultInputData.GetMoveInput();

   
    FVector DashDirection = MoveIntent.IsNearlyZero() ? ZeroInputsData.LookDir.Vector().GetSafeNormal2D() : MoveIntent.GetSafeNormal();
    
    if (DashDirection.IsNearlyZero())
    {
        DashDirection = GetActorForwardVector();
    }

    DashMove->Velocity = DashDirection * DashSpeed;
    DashMove->DurationMs = DurationSeconds * 1000.0f;
    DashMove->MixMode = EMoveMixMode::OverrideVelocity;

    MoverComponent->QueueLayeredMove(DashMove);
}