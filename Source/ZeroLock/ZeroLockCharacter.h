// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "GameplayAbilitySpec.h"
#include "ZeroLock.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZeroLockCharacter.generated.h"

//enum class EGASAbilityInputID;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangeDelgate,float ,currentHealth,float,MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStunChanged,bool,IsStunned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageRecievedDelegate,float,currentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddAbilityIconDelegate,UBaseGameplayAbility*,AbilitytoAdd,EGASAbilityInputID,slotToAddIn);

USTRUCT()
struct FMyAbilityMap
{
	GENERATED_BODY()

	UPROPERTY()
	UBaseGameplayAbility* Ability;

	UPROPERTY()
	EGASAbilityInputID InputID;
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parry, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ParryComp;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Melee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Parry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Confirm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EI_Cancel;
	
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
	void MeleePressed();
	void MeleeReleased();
	void ParryPressed();


public:
	float MeleePressedTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Melee, meta = (AllowPrivateAccess = "true"))
	float MeleeMinHoldTime;
	FTimerHandle MeleePressedTimer;
	bool bMeleeUsed = false;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Stunned(FGameplayTag GameplayTag, int NewCount);
	void Parry(FGameplayTag GameplayTag, int NewCount);


	void HealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void AmmoAttributeChange(const FOnAttributeChangeData& OnAttributeChangeData);
	
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

	//
	void Death();

	//Ability System 
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBaseCharAbilitySystemComponent* AbilitySystemComp;


	UPROPERTY()
	class UBaseCharAttributeSet* AttributeSet;

public:
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	virtual UBaseCharAbilitySystemComponent* GetMyAbilitySystemComp()const;

	virtual UBaseCharAttributeSet* GetMyAttributeSet()const;

	virtual void InitializeAttributes();
	UFUNCTION()
	void NewAbilityAddedLocal(FGameplayAbilitySpec& AbilitySpec);
	virtual void GiveAbilities();

	virtual void BroadcastAbilitiesToUI(UBaseGameplayAbility* Ability , EGASAbilityInputID InputID);

	void GrantAbilityOfClassX(TSubclassOf<class UBaseGameplayAbility> AbilityToGrant,EGASAbilityInputID InputToBindTo,bool brodcast = false);


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
	void SecondryFireReleased();

	void Ability_1Pressed();
	void Ability_1Released();

	void Ability_2Pressed();
	void Ability_2Released();

	void UltimateAbilityPressed();
	void UltimateAbilityReleased();

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS/Melee")
	TSubclassOf<class UBaseGameplayAbility> LightMeleeAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS/Melee")
	TSubclassOf<class UBaseGameplayAbility> HeavyMeleeAbility;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS/Melee")
	TSubclassOf<class UBaseGameplayAbility> ParryAbility;



	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GAS")
	TArray<FGameplayAbilitySpecHandle> DefaultAbilitiesHandles;

	UPROPERTY(BlueprintAssignable)
	FHealthChangeDelgate HealthChangeDelegate;
	UPROPERTY(BlueprintAssignable)
	FHealthChangeDelgate AmmoChangeDelegate;
	UPROPERTY(BlueprintAssignable)
	FStunChanged StunChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FDamageRecievedDelegate DamageRecievedDelegate;
	UPROPERTY(BlueprintAssignable)
	FAddAbilityIconDelegate AddAbilityIconDelegate;

	
	
public:
	UFUNCTION()
	void HealthChanged(float currentH , float MaxH);

	UFUNCTION()
	void OnTakeDamage(float currentH);

	UFUNCTION()
	void HandleDeath();

	UPROPERTY(Replicated)
	bool bIsDead = false;

	UFUNCTION()
	void OnDied(AController* Killer, AController* Victim);

	void ResetCharacter(FVector Location);
	void PassiveAbilityRestart(TSubclassOf<class UBaseGameplayAbility> AbilityToGrant);
	void ResetAllAbilities();

	UPROPERTY()
	TArray<FMyAbilityMap> AbilitiesArray;


	UFUNCTION()
	void OnRep_AbilityUIData();
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Animation/melee")
	UAnimMontage* LightMeleeMontage;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Animation/melee")
	UAnimMontage* HeavyMeleeMontage;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FOVChange(float newFOV,float duration);



	TMap<EGASAbilityInputID,FGameplayTag> inputTags;

	void InitInputTagsMap();
};


