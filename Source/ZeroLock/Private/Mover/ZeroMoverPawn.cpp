// Copyright Preetham Mukundan (C) 2026


#include "Mover/ZeroMoverPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
    SpringArmComponent->TargetArmLength = 400.0f; // Typical 3rd person distance
    SpringArmComponent->bUsePawnControlRotation = true; // Rotate arm based on controller pitch/yaw

    // 5. Camera
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false; // Camera itself doesn't rotate, the arm does
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
        // Get the active mode name (e.g., "Walking", "Falling", or your custom "SlideMode")
        FName CurrentMode = MoverComponent->GetMovementModeName();
        
        // Print it to the screen every frame (0.0f duration means it refreshes instantly)
        GEngine->AddOnScreenDebugMessage(
            1, // Use a consistent key so it overwrites instead of spamming the screen
            0.0f, 
            FColor::Cyan, 
            FString::Printf(TEXT("Current Mode: %s"), *CurrentMode.ToString())
        );
    }
}


void AZeroMoverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 1. Map the Context via the Local Player Subsystem
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // 2. Bind the Actions
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            // We use Triggered so the value updates continuously while held, and Completed to zero it out
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnMove);
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AZeroMoverPawn::OnMove);
             EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroMoverPawn::OnLook);
            
            EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&AZeroMoverPawn::OnJumpPressed);
            EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AZeroMoverPawn::OnJumpReleased);
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
        DefaultInputs.bIsJumpJustPressed=false;
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

    // 3. Assign the intent using the new setter and enum
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
}

void AZeroMoverPawn::OnMove(const FInputActionValue& Value)
{
    // CACHE the intent. Do not apply movement here.
    CachedMoveInput = Value.Get<FVector2D>();
}

void AZeroMoverPawn::OnLook(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // This safely modifies the controller rotation. 
    // The Mover plugin will read the Controller rotation for Orientation Intent.
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

