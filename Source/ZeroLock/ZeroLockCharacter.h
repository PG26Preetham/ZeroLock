// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "GameplayAbilitySpec.h"
#include "ZeroLockCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangeDelgate,float ,currentHealth,float,MaxHealth);

UCLASS(config=Game)
class AZeroLockCharacter : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Movement")
	class UZeroBaseCharacterMovementComp* ZeroMovementComp;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;



	//Gameplay Ability system inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_PrimaryFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Reload;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_SecondryFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Ability1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Ability2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Ultimate;

	
public:
	AZeroLockCharacter(const FObjectInitializer& ObjectInitializer);
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


	void DashPressed();
	void DashReleased();
	void CrouchPressed();
	void CrouchReleased();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	//movement
	FCollisionQueryParams GetIgnoreCharacterParams() const ;

	virtual bool CanJumpInternal_Implementation() const override;

	
	//JUMP
	bool bPressedZeroJump;
	bool bStillJumpKeyDown =false;
	//float ZeroTimeJumpKeyPressed;
	float ZeroJumpHoldTIme;

	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void ClearJumpInput(float DeltaTime) override;

	//Ability System 
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBaseCharAbilitySystemComponent* AbilitySystemComp;


	UPROPERTY()
	class UBaseCharAttributeSet* AttributeSet;

public:
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();


	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	bool bIsPrimaryPressed =false;
	void PrimaryFirePressed();
	void PrimaryFireReleased();
	float TimeOfLastShot;

	void ChangeFireRate();

	void PrimaryFireTickFunction();
	FTimerHandle PrimaryFireTickHandle;

	void SecondryFirePressed();

	void Ability_1Pressed();

	void Ability_2Pressed();

	void UltimateAbilityPressed();

	void Reload();
	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultGameplayEffect;



	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> PrimaryFireAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> SecondryFireAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> Ability_1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> Ability_2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> UltimateAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UBaseGameplayAbility> ReloadAbility;



	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GAS")
	TArray<FGameplayAbilitySpecHandle> DefaultAbilitiesHandles;

	UPROPERTY(BlueprintAssignable)
	FHealthChangeDelgate HealthChangeDelegate;
public:
	UFUNCTION()
	void HealthChanged(float currentH , float MaxH);
};

