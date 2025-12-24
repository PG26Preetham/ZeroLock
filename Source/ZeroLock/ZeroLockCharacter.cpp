// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZeroLockCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
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
#include "Zero_BasePlayerController.h"
#include "Gamemode/Zero_BaseGameModeBase.h"
#include "ZeroLock/Public/ZeroBaseCharacterMovementComp.h"
#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "GAS/BaseGameplayAbility.h"
#include "GAS/ZL_GameplayTags.h"
#include "Items/Zero_Item_Inventory_Component.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/MVVM/ZL_VM_Attributes.h"
#include "ZeroLock/Public/Movement/Zero_ZiplineActor.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AZeroLockCharacter

void AZeroLockCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (ZeroMovementComp->MovementMode == MOVE_Custom)
	{
		if (ZeroMovementComp->IsCustomMovementMode(ECustomMovementMode::CMOVE_Slide))
		{
			VM_Attributes->SetIsInfiniteAmmo(true);
		}
	}
	if (PreviousCustomMode == ECustomMovementMode::CMOVE_Slide)
	{
		VM_Attributes->SetIsInfiniteAmmo(false);
	}
}

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

	ItemInventoryComp = CreateDefaultSubobject<UZero_Item_Inventory_Component>(TEXT("ItemInventory"));
	ItemInventoryComp->SetIsReplicated(true);

	ParryComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParryComponent"));
	ParryComp->SetupAttachment(RootComponent);
	ParryComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZeroLockCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	ParryComp->SetVisibility(false);
	CreateVM_Att();
	//AttributeSet->OnCharacterDied.AddUniqueDynamic(this,&ThisClass::AZeroLockCharacter::OnDied);

	
	
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

void AZeroLockCharacter::Death()
{
	
	
}

class UAbilitySystemComponent* AZeroLockCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

UBaseCharAbilitySystemComponent* AZeroLockCharacter::GetMyAbilitySystemComp() const
{
	return AbilitySystemComp;
}

UBaseCharAttributeSet* AZeroLockCharacter::GetMyAttributeSet() const
{
	return AttributeSet;
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
		FGameplayTag StunTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Stun"),false);
		FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag(FName("ZeroLock.Melee.Parry"),false);
		AbilitySystemComp->RegisterGameplayTagEvent(StunTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AZeroLockCharacter::Stunned);
		AbilitySystemComp->RegisterGameplayTagEvent(ParryTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AZeroLockCharacter::Parry);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).AddUObject(this,&AZeroLockCharacter::HealthAttributeChanged);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaximumHealthAttribute()).AddUObject(this,&AZeroLockCharacter::HealthAttributeChanged);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxAmmoAttribute()).AddUObject(this,&AZeroLockCharacter::AmmoAttributeChange);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentAmmoAttribute()).AddUObject(this,&AZeroLockCharacter::AmmoAttributeChange);
		
		AbilitySystemComp->OnNewAbilityAdded.AddUniqueDynamic(this,&ThisClass::NewAbilityAddedLocal);
	}
}

void AZeroLockCharacter::NewAbilityAddedLocal(FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsLocallyControlled()) return;
	
	
	UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(AbilitySpec.GetPrimaryInstance());
	EGASAbilityInputID InputID = static_cast<EGASAbilityInputID>(AbilitySpec.InputID);
	if (Ability)
	{
		
		AbilitiesArray.Add(FMyAbilityMap(Ability, InputID));
		BroadcastAbilitiesToUI(Ability,InputID);
	}
}

void AZeroLockCharacter::GiveAbilities()
{
	InitInputTagsMap();
	
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
		
		GrantAbilityOfClassX(SecondryFireAbility,EGASAbilityInputID::Secondry_Attack,true);
		GrantAbilityOfClassX(Ability_1,EGASAbilityInputID::Ability_1,true);
		GrantAbilityOfClassX(Ability_2,EGASAbilityInputID::Ability_2,true);
		GrantAbilityOfClassX(UltimateAbility,EGASAbilityInputID::Ultimate,true);
		GrantAbilityOfClassX(ReloadAbility,EGASAbilityInputID::Reload);
		GrantAbilityOfClassX(HeavyMeleeAbility,EGASAbilityInputID::Melee);
		GrantAbilityOfClassX(ParryAbility,EGASAbilityInputID::Parry);

		
	}
	if (HasAuthority() && AbilitySystemComp)
	{
		for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : DefaultAbilities)
		{

			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));


		}
	}
}

void AZeroLockCharacter::BroadcastAbilitiesToUI(UBaseGameplayAbility* Ability , EGASAbilityInputID InputID)
{	
		AbilitiesArray.Add(FMyAbilityMap(Ability , InputID));
}




void AZeroLockCharacter::GrantAbilityOfClassX(TSubclassOf<class UBaseGameplayAbility> AbilityToGrant,EGASAbilityInputID InputToBindTo, bool brodcast)
{
	if (AbilityToGrant)
	{
		
		EGASAbilityInputID AbiltyInputID = InputToBindTo;
		FGameplayAbilitySpecHandle GrantedHandle;
		FGameplayAbilitySpec GrantedSpec=FGameplayAbilitySpec(AbilityToGrant, 1, static_cast<int32>(AbiltyInputID), this);
		if (inputTags.Contains(AbiltyInputID))
		{
			GrantedSpec.DynamicAbilityTags.AddTag(inputTags.FindRef(InputToBindTo));
		}
		
		
		if (AbilityToGrant.GetDefaultObject()->TargetStyle == EGASTargetConfirmationStyle::Passive)
		{
			EGASAbilityInputID AbiltyInputIDX = EGASAbilityInputID::None;
			GrantedSpec.InputID = static_cast<int32>(AbiltyInputIDX);
			GrantedHandle =AbilitySystemComp->GiveAbility(GrantedSpec);
			DefaultAbilitiesHandles.Add(GrantedHandle);
			//GetAbilitySystemComponent()->TryActivateAbilityByClass(AbilityToGrant);
		}
		else
		{
			GrantedHandle = AbilitySystemComp->GiveAbility(GrantedSpec);
			DefaultAbilitiesHandles.Add(GrantedHandle);
		}
		FGameplayAbilitySpec* Spec = GetAbilitySystemComponent()->FindAbilitySpecFromHandle(GrantedHandle);
		UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(Spec->Ability);
		if (Ability)
		{
			Ability->SetInputID(InputToBindTo);
		}
		
	}
	
}

UZero_Item_Inventory_Component* AZeroLockCharacter::GetInventoryComponent() const
{
	return ItemInventoryComp;
}

void AZeroLockCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//server GAS 
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	//CreateVM_Att();
	InitializeAttributes();
	GiveAbilities();
	
}

void AZeroLockCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Client GAS
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	InitializeAttributes();

	CreateVM_Att();
}

void AZeroLockCharacter::PrimaryFirePressed()
{
	if(!PrimaryFireAbility)
	{
		return;
	}
	if (!AttributeSet)
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

bool AZeroLockCharacter::IsAlive()
{
	if (AttributeSet)
	{
		return AttributeSet->GetCurrentHealth() > 0;
	}
	return false;
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
	if (AttributeSet->GetCurrentAmmo() > 0)
	{
		AbilitySystemComp->TryActivateAbilityByClass(PrimaryFireAbility, true);
		TimeOfLastShot = GetWorld()->TimeSeconds;
	}
	else
	{
		PrimaryFireReleased();
		//Reload();
	}
	
}

void AZeroLockCharacter::SecondryFirePressed()
{
	GetAbilitySystemComponent()->TryActivateAbilityByClass(SecondryFireAbility);
}

void AZeroLockCharacter::SecondryFireReleased()
{
}

void AZeroLockCharacter::Ability_1Pressed()
{
	//GetAbilitySystemComponent()->TryActivateAbilityByClass(Ability_1);
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability_1));
}

void AZeroLockCharacter::Ability_1Released()
{
	GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(EGASAbilityInputID::Ability_1));
}

void AZeroLockCharacter::Ability_2Pressed()
{
	//GetAbilitySystemComponent()->TryActivateAbilityByClass(Ability_2);
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability_2));
}

void AZeroLockCharacter::Ability_2Released()
{
	GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(EGASAbilityInputID::Ability_2));
}

void AZeroLockCharacter::UltimateAbilityPressed()
{
	//GetAbilitySystemComponent()->TryActivateAbilityByClass(UltimateAbility);
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ultimate));
}

void AZeroLockCharacter::UltimateAbilityReleased()
{
	GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(EGASAbilityInputID::Ultimate));
}

void AZeroLockCharacter::Reload()
{
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Reload));
}

void AZeroLockCharacter::HealthChanged(float currentH , float MaxH)
{
	if (HealthChangeDelegate.IsBound())
	{
		HealthChangeDelegate.Broadcast(currentH, MaxH);
	}
	if (UZL_VM_Attributes* VM =GetVM_Attributes())
	{
		VM->SetHealth(currentH);
		VM->SetMaxHealth(MaxH);
	}
}

void AZeroLockCharacter::OnTakeDamage(float currentH)
{
	if (DamageRecievedDelegate.IsBound())
	{
		DamageRecievedDelegate.Broadcast(currentH);
	}

}

void AZeroLockCharacter::AddLastHit(AZeroLockCharacter* Character)
{
	if (!IsAlive())return;
	if (!Character) return;
	float CurrentTime = GetWorld()->GetTimeSeconds();
	LastHitCharacter = Character;

	if (AssistTimeMap.Contains(Character))
	{
		AssistTimeMap.Remove(Character);
		AssistTimeMap.Add(Character,CurrentTime);
	}
	if (!AssistListCharacters.Contains(Character))
	{
		AssistListCharacters.Add(Character);
	}
	ClearAssistList(CurrentTime);
	
}

void AZeroLockCharacter::ClearAssistList(float currentTime)
{
	TArray<AZeroLockCharacter*> ToRemove;

	for (auto& Pair : AssistTimeMap)
	{
		if (currentTime - Pair.Value > AssistWindow)
		{
			ToRemove.Add(Pair.Key);
		}
	}

	for (AZeroLockCharacter* Entry : ToRemove)
	{
		AssistListCharacters.Remove(Entry);
		AssistTimeMap.Remove(Entry);
	}
}

void AZeroLockCharacter::OnDied(AController* Killer, AController* Victim)
{
	ZLOG("Death");
	if (!AbilitySystemComp->GetOwner()->HasAuthority()) return;

	// Cancel abilities
	AbilitySystemComp->CancelAllAbilities();

	// Remove all buffs/debuffs
	AbilitySystemComp->RemoveActiveEffectsWithTags(FGameplayTagContainer());

	// notify GameMode
	if (AZero_BaseGameModeBase* GM = GetWorld()->GetAuthGameMode<AZero_BaseGameModeBase>())
	{
		GM->Killed(Killer, Victim);
	}
		
}



void AZeroLockCharacter::ResetCharacter()
{
	
	ZLOG("ResetCharacter");
	AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(GetController());
	SetActorLocation(PC->SelectedStartLocation);

	if (!AbilitySystemComp->GetOwner()->HasAuthority()) return;

	// Cancel abilities
	AbilitySystemComp->CancelAllAbilities();

	// Remove all buffs/debuffs
	AbilitySystemComp->RemoveActiveEffectsWithTags(FGameplayTagContainer());
	AbilitySystemComp->RemoveActiveEffects(FGameplayEffectQuery()); 

	

	

	// Reactivate abilities if needed (passive abilities)
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	ResetAllAbilities();

	float Max = AttributeSet->GetMaximumHealth();
	AttributeSet->SetCurrentHealth(Max);
	if (PC) EnableInput(PC);
	bIsDead = false;
}

void AZeroLockCharacter::PassiveAbilityRestart(TSubclassOf<class UBaseGameplayAbility> AbilityToGrant)
{
	if (!AbilityToGrant) return;
	if (AbilityToGrant.GetDefaultObject()->TargetStyle == EGASTargetConfirmationStyle::Passive)
	{
		AbilitySystemComp->TryActivateAbilityByClass(AbilityToGrant);
	}
}

void AZeroLockCharacter::ResetAllAbilities()
{
	PassiveAbilityRestart(SecondryFireAbility);
	PassiveAbilityRestart(Ability_1);
	PassiveAbilityRestart(Ability_2);
	PassiveAbilityRestart(UltimateAbility);
	for (TSubclassOf<UBaseGameplayAbility> ability : DefaultAbilities)
	{
		PassiveAbilityRestart(ability);
	}
}

void AZeroLockCharacter::HandleDeath()
{
	if (bIsDead) return;
	APlayerController* PC = Cast<APlayerController>(GetController());
	
	AbilitySystemComp->CancelAllAbilities();
	// Remove all buffs/debuffs
	AbilitySystemComp->RemoveActiveEffectsWithTags(FGameplayTagContainer());
	bIsDead = true;
		
	if (PC)
	{
		DisableInput(PC);

		ServerHandleDeath(PC);
	}
		

}

void AZeroLockCharacter::ServerHandleDeath_Implementation(APlayerController* PC)
{
	if (AZero_BaseGameModeBase* GM = GetWorld()->GetAuthGameMode<AZero_BaseGameModeBase>())
	{
		GM->HandlePlayerDeath(this, PC);
	}
}


void AZeroLockCharacter::OnRep_AbilityUIData()
{
	for (FMyAbilityMap unit : AbilitiesArray)
	{
		AddAbilityIconDelegate.Broadcast(unit.Ability,unit.InputID);
	}
}

void AZeroLockCharacter::InitInputTagsMap()
{
	inputTags.Add(EGASAbilityInputID::Secondry_Attack,ZerolockGameplayTagsForBinding::TAG_INPUT_SECONDRY);
	inputTags.Add(EGASAbilityInputID::Ability_1,ZerolockGameplayTagsForBinding::TAG_INPUT_ABILITY_1);
	inputTags.Add(EGASAbilityInputID::Ability_2,ZerolockGameplayTagsForBinding::TAG_INPUT_ABILITY_2);
	inputTags.Add(EGASAbilityInputID::Ultimate,ZerolockGameplayTagsForBinding::TAG_INPUT_ULTIMATE);
}

void AZeroLockCharacter::HandleWeaponHitEvent(const FGameplayEventData& EventData)
{
	OnWeaponHitEventReceived.Broadcast(EventData);
}

UZL_VM_Attributes* AZeroLockCharacter::GetVM_Attributes()
{
	if (!VM_Attributes)
	{
		CreateVM_Att();
	}
	return VM_Attributes;
}

UObject* AZeroLockCharacter::GetVMObject_Attributes()
{
	return GetVM_Attributes();
}

void AZeroLockCharacter::CreateVM_Att()
{
	if (VM_Attributes) return;
	VM_Attributes = NewObject<UZL_VM_Attributes>(this);

	// Immediately sync the current GAS values to the new VM
	if (AttributeSet)
	{
		VM_Attributes->SetHealth(AttributeSet->GetCurrentHealth());
		VM_Attributes->SetMaxHealth(AttributeSet->GetMaximumHealth());
		VM_Attributes->SetAmmo(static_cast<int32>(AttributeSet->GetCurrentAmmo()));
		VM_Attributes->SetMaxAmmo(static_cast<int32>(AttributeSet->GetMaxAmmo()));
		VM_Attributes->SetIsInfiniteAmmo(false);
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
		EnhancedInputComponent->BindAction(EI_SecondryFire,ETriggerEvent::Completed,this,&AZeroLockCharacter::SecondryFireReleased);

		EnhancedInputComponent->BindAction(EI_Ability1,ETriggerEvent::Started,this,&AZeroLockCharacter::Ability_1Pressed);
		EnhancedInputComponent->BindAction(EI_Ability1,ETriggerEvent::Completed,this,&AZeroLockCharacter::Ability_1Released);

		EnhancedInputComponent->BindAction(EI_Ability2,ETriggerEvent::Started,this,&AZeroLockCharacter::Ability_2Pressed);
		EnhancedInputComponent->BindAction(EI_Ability2,ETriggerEvent::Completed,this,&AZeroLockCharacter::Ability_2Released);

		EnhancedInputComponent->BindAction(EI_Ultimate,ETriggerEvent::Started,this,&AZeroLockCharacter::UltimateAbilityPressed);
		EnhancedInputComponent->BindAction(EI_Ultimate,ETriggerEvent::Completed,this,&AZeroLockCharacter::UltimateAbilityReleased);
		
		EnhancedInputComponent->BindAction(EI_Reload,ETriggerEvent::Started,this,&AZeroLockCharacter::Reload);

		EnhancedInputComponent->BindAction(EI_Melee,ETriggerEvent::Started,this,&AZeroLockCharacter::MeleePressed);
		EnhancedInputComponent->BindAction(EI_Melee,ETriggerEvent::Completed,this,&AZeroLockCharacter::MeleeReleased);
		
		EnhancedInputComponent->BindAction(EI_Parry,ETriggerEvent::Started,this,&AZeroLockCharacter::ParryPressed);

		EnhancedInputComponent->BindAction(EI_Confirm,ETriggerEvent::Completed,AbilitySystemComp,&UAbilitySystemComponent::LocalInputConfirm);
		EnhancedInputComponent->BindAction(EI_Cancel,ETriggerEvent::Completed,AbilitySystemComp,&UAbilitySystemComponent::LocalInputCancel);
		
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
	DOREPLIFETIME(AZeroLockCharacter,bIsDead);
	DOREPLIFETIME(AZeroLockCharacter,StartLocation);
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
		if (StunChangedDelegate.IsBound())
		{
			StunChangedDelegate.Broadcast(true);
		}
	}
	else
	{
		if (PC)
		{
			EnableInput(PC);
		}
		if (StunChangedDelegate.IsBound())
		{
			StunChangedDelegate.Broadcast(false);
		}
	}
}

void AZeroLockCharacter::Parry(FGameplayTag GameplayTag, int NewCount)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (NewCount>0)
	{
		ParryComp->SetVisibility(true);
		if (PC)
		{
			DisableInput(PC);
		}
	}
	else
	{
		ParryComp->SetVisibility(false);
		if (PC)
        {
			EnableInput(PC);
        }
	}
}

void AZeroLockCharacter::HealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (!AttributeSet) return;
	
	float currentH= AttributeSet->GetCurrentHealth();
	float MaxH = AttributeSet->GetMaximumHealth();
	
	
	if (HealthChangeDelegate.IsBound())
	{
		HealthChangeDelegate.Broadcast(currentH, MaxH);
	}
	if (currentH <= 0.0f && currentH < MaxH)
	{
		//HandleDeath();
	}
	OnTakeDamage(currentH);
	if (UZL_VM_Attributes* VM =GetVM_Attributes())
	{
		VM->SetHealth(currentH);
		VM->SetMaxHealth(MaxH);
	}
}

void AZeroLockCharacter::AmmoAttributeChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (!AttributeSet) return;
	float currentA= AttributeSet->GetCurrentAmmo();
	float MaxA = AttributeSet->GetMaxAmmo();
	VM_Attributes->SetMaxAmmo(static_cast<int32>(MaxA));
	VM_Attributes->SetAmmo(static_cast<int32>(currentA));
	if (AmmoChangeDelegate.IsBound())
	{
		
		//AmmoChangeDelegate.Broadcast(currentA, MaxA);
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

void AZeroLockCharacter::MeleePressed()
{
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Melee));
}

void AZeroLockCharacter::MeleeReleased()
{
	GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(EGASAbilityInputID::Melee));	
}

void AZeroLockCharacter::ParryPressed()
{
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Parry));
}
