// Copyright Preetham Mukundan (C) 2026

#include "Mover/ZeroMoverPawn.h"
#include "Mover/ZeroMovementData.h" // <-- ADD THIS: Includes FZeroMovementInputs

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DefaultMovementSet/LayeredMoves/MultiJumpLayeredMove.h"
#include "ZeroLock/ZeroLock.h"

AZeroMoverPawn::AZeroMoverPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

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

void AZeroMoverPawn::BeginPlay()
{
    Super::BeginPlay();
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
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnMove);
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AZeroMoverPawn::OnMove);
            
            EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnJumpPressed);
            EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AZeroMoverPawn::OnJumpReleased);
            
            EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnCrouchPressed);
            EnhancedInputComponent->BindAction(CrouchAction,ETriggerEvent::Completed,this,&AZeroMoverPawn::OnCrouchReleased);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnLook);
        }
    }
}

void AZeroMoverPawn::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
    // 1. Get a mutable reference directly from the collection (creates it if it doesn't exist)
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
   
    // 2. Translate 2D WASD input into a world-space 3D direction based on camera facing
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

    // 3. Assign the intent for Epic's Default Walk Mode
    DefaultInputs.SetMoveInput(EMoveInputType::DirectionalIntent, ControlVector); 

    // 4. Set Control Rotation and Orientation Intent
    if (Controller)
    {
        DefaultInputs.ControlRotation = Controller->GetControlRotation();
        DefaultInputs.OrientationIntent = DefaultInputs.ControlRotation.Vector();
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

    // 3. Detect a clean "Just Pressed" event locally
    bool bCustomJump = (bLocalJumpPressed && !bWasJumpPressedLastFrame);
    bWasJumpPressedLastFrame = bLocalJumpPressed;

    // 4. Validate the Air Jump Intent
    bool bValidAirJump = false;
    if (bCustomJump && CurrentMode == DefaultModeNames::Falling && LocalAirJumpsUsed < SlideSettings->MaxAirJumps)
    {
        bValidAirJump = true;
        LocalAirJumpsUsed++;
    }
    // ---------------------------------------------------------------------------------
    // 5. INJECT ZERO LOCK CUSTOM INPUTS
    // We add our custom input struct alongside Epic's default one. 
    // Our ZeroSlideTransition and ZeroSlideMode will look for THIS specific struct.
    // ---------------------------------------------------------------------------------
    FZeroMovementInputs& ZeroInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FZeroMovementInputs>();
    ZeroInputs.MoveInput = ControlVector;
    ZeroInputs.bWantsToCrouch = bCachedWantsToCrouch;
    ZeroInputs.bSlideIntentValid = bLocalSlideIntentValid;
    ZeroInputs.bCustomJumpJustPressed = bCustomJump;
    ZeroInputs.bWantsToAirJump = bValidAirJump;
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
    // Keep the local flag for the base ground jump
    bLocalJumpPressed = true;
/*
    if (!MoverComponent) return;

    FName CurrentMode = MoverComponent->GetMovementModeName();

        bCachedWantsToCrouch = false;

        TSharedPtr<FLayeredMove_MultiJump> DoubleJumpMove = MakeShared<FLayeredMove_MultiJump>();
        DoubleJumpMove->UpwardsSpeed = SlideSettings ? SlideSettings->AirJumpForce : 600.0f;
        DoubleJumpMove->MaximumInAirJumps = SlideSettings ? SlideSettings->MaxAirJumps : 1;
        DoubleJumpMove->MixMode = EMoveMixMode::OverrideVelocity;

        
        MoverComponent->QueueLayeredMove(DoubleJumpMove);*/
    
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