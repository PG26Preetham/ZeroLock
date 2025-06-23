// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZeroLockCharacter.h"

#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ZeroLock/Public/ZeroBaseCharacterMovementComp.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "GAS/BaseGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/Public/Movement/Zero_ZiplineActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AZeroLockCharacter

AZeroLockCharacter::AZeroLockCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UZeroBaseCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{

	ZeroMovementComp = Cast<UZeroBaseCharacterMovementComp>(GetCharacterMovement());
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	JumpMaxCount =2;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//GAS
	
	//GAS Components
	AbilitySystemComp = CreateDefaultSubobject<UBaseCharAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AttributeSet = CreateDefaultSubobject<UBaseCharAttributeSet>(TEXT("AttributeSet"));
}

void AZeroLockCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	FGameplayTag StunTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Stun"),false);
	AbilitySystemComp->RegisterGameplayTagEvent(StunTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AZeroLockCharacter::Stunned);
}

FCollisionQueryParams AZeroLockCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams params;

	TArray<AActor*> CharacterChilder;
	GetAllChildActors(CharacterChilder);
	params.AddIgnoredActors(CharacterChilder);
	params.AddIgnoredActor(this);

	return params;
}

bool AZeroLockCharacter::CanJumpInternal_Implementation() const
{
	if(ZeroMovementComp->IsCustomMovementMode(CMOVE_Zipline))
	{
		return true;
	}
	if(ZeroMovementComp->IsCustomMovementMode(CMOVE_Slide))
	{
		return true;
	}
	return Super::CanJumpInternal_Implementation();
}

void AZeroLockCharacter::Jump()
{
	Super::Jump();

	bPressedZeroJump = true;

	bPressedJump = false;
	bStillJumpKeyDown =true;
	//ZeroTimeJumpKeyPressed = GetWorld()->TimeSeconds;
	ZeroJumpHoldTIme =0.0f;
}

void AZeroLockCharacter::StopJumping()
{
	Super::StopJumping();
	bPressedZeroJump = false;
	bStillJumpKeyDown = false;
}

void AZeroLockCharacter::ClearJumpInput(float DeltaTime)
{
	Super::ClearJumpInput(DeltaTime);
	if(bStillJumpKeyDown)
	{
		ZeroJumpHoldTIme += DeltaTime;
	}
	else
	{
		//	ZeroJumpHoldTIme =0;
	}
}

class UAbilitySystemComponent* AZeroLockCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AZeroLockCharacter::InitializeAttributes()
{
	
	if (AbilitySystemComp && DefaultGameplayEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);


		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(DefaultGameplayEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AZeroLockCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComp)
	{
		for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			DefaultAbilitiesHandles.Add(
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this)));
		}
		if (PrimaryFireAbility)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(PrimaryFireAbility, 1, static_cast<int32>(PrimaryFireAbility.GetDefaultObject()->AbilityInputID), this)));
		}
		if (SecondryFireAbility)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(SecondryFireAbility, 1, static_cast<int32>(SecondryFireAbility.GetDefaultObject()->AbilityInputID), this)));
		}
		if (Ability_1)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(Ability_1, 1, static_cast<int32>(Ability_1.GetDefaultObject()->AbilityInputID), this)));
		}
		if (Ability_2)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(Ability_2, 1, static_cast<int32>(Ability_2.GetDefaultObject()->AbilityInputID), this)));
		}
		if (UltimateAbility)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(UltimateAbility, 1, static_cast<int32>(UltimateAbility.GetDefaultObject()->AbilityInputID), this)));
		}
		if (ReloadAbility)
		{
			DefaultAbilitiesHandles.Add(
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(ReloadAbility, 1, static_cast<int32>(ReloadAbility.GetDefaultObject()->AbilityInputID), this)));
		}
		
	}
	if (HasAuthority() && AbilitySystemComp)
	{
		for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : DefaultAbilities)
		{

			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));


		}
	}
}

void AZeroLockCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//server GAS 
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
}

void AZeroLockCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Client GAS
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	InitializeAttributes();
}

void AZeroLockCharacter::PrimaryFirePressed()
{
	if(!PrimaryFireAbility)
	{
		return;
	}
	bIsPrimaryPressed = true;
	float FirstDelay =FMath::Max(TimeOfLastShot + AttributeSet->FireRate.GetCurrentValue() - GetWorld()->TimeSeconds,0.0f);
	FString TheFloatStr = "Dam=" + FString::SanitizeFloat(FirstDelay);
	float FireRate = AttributeSet->FireRate.GetCurrentValue();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, *TheFloatStr);
	GetWorldTimerManager().SetTimer(PrimaryFireTickHandle,this,&AZeroLockCharacter::PrimaryFireTickFunction,FireRate , true, FirstDelay);
	//GetWorldTimerManager().SetTimer(PrimaryFireTickHandle,this,&AZeroLockCharacter::PrimaryFireTickFunction, AttributeSet->FireRate, true, FirstDelay);
}

void AZeroLockCharacter::PrimaryFireReleased()
{
	bIsPrimaryPressed = false;
	GetWorldTimerManager().ClearTimer(PrimaryFireTickHandle);
	//GetWorldTimerManager().
}

void AZeroLockCharacter::ChangeFireRate()
{
	if (!bIsPrimaryPressed) return;
	float FirstDelay =FMath::Max(TimeOfLastShot + AttributeSet->FireRate.GetCurrentValue() - GetWorld()->TimeSeconds,0.0f);
	
	float FireRate = AttributeSet->FireRate.GetCurrentValue();
	GetWorldTimerManager().SetTimer(PrimaryFireTickHandle,this,&AZeroLockCharacter::PrimaryFireTickFunction,FireRate , true, FirstDelay);
	
}

void AZeroLockCharacter::PrimaryFireTickFunction()
{
	AbilitySystemComp->TryActivateAbilityByClass(PrimaryFireAbility, true);
	TimeOfLastShot = GetWorld()->TimeSeconds;
}

void AZeroLockCharacter::SecondryFirePressed()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(SecondryFireAbility);
}

void AZeroLockCharacter::Ability_1Pressed()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(Ability_1);
}

void AZeroLockCharacter::Ability_2Pressed()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(Ability_2);
}

void AZeroLockCharacter::UltimateAbilityPressed()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(UltimateAbility);
}

void AZeroLockCharacter::Reload()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(ReloadAbility);
}

void AZeroLockCharacter::HealthChanged(float currentH , float MaxH)
{
	if (HealthChangeDelegate.IsBound())
	{
		HealthChangeDelegate.Broadcast(currentH, MaxH);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AZeroLockCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AZeroLockCharacter::DashPressed);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AZeroLockCharacter::DashReleased);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AZeroLockCharacter::CrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AZeroLockCharacter::CrouchReleased);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZeroLockCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZeroLockCharacter::Look);

		//GAS
		EnhancedInputComponent->BindAction(EI_PrimaryFire,ETriggerEvent::Started,this,&AZeroLockCharacter::PrimaryFirePressed);
		EnhancedInputComponent->BindAction(EI_PrimaryFire,ETriggerEvent::Completed,this,&AZeroLockCharacter::PrimaryFireReleased);

		EnhancedInputComponent->BindAction(EI_SecondryFire,ETriggerEvent::Started,this,&AZeroLockCharacter::SecondryFirePressed);

		EnhancedInputComponent->BindAction(EI_Ability1,ETriggerEvent::Started,this,&AZeroLockCharacter::Ability_1Pressed);

		EnhancedInputComponent->BindAction(EI_Ability2,ETriggerEvent::Started,this,&AZeroLockCharacter::Ability_2Pressed);

		EnhancedInputComponent->BindAction(EI_Ultimate,ETriggerEvent::Started,this,&AZeroLockCharacter::UltimateAbilityPressed);
		
		EnhancedInputComponent->BindAction(EI_Reload,ETriggerEvent::Started,this,&AZeroLockCharacter::Reload);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel",  FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("EGASAbilityInputID"))), static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));

		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AZeroLockCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AZeroLockCharacter::Stunned(FGameplayTag GameplayTag, int NewCount)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (NewCount>0)
	{
		AbilitySystemComp->CancelAllAbilities();
		
		if (PC)
		{
			DisableInput(PC);
		}
			
	}
	else
	{
		if (PC)
		{
			EnableInput(PC);
		}
	}
}


void AZeroLockCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AZeroLockCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AZeroLockCharacter::DashPressed()
{
	ZeroMovementComp->DashPressed();
}

void AZeroLockCharacter::DashReleased()
{
	ZeroMovementComp->DashReleased();
}

void AZeroLockCharacter::CrouchPressed()
{
	ZeroMovementComp->CrouchPressed();
}

void AZeroLockCharacter::CrouchReleased()
{
	ZeroMovementComp->CrouchReleased();
}
