// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZeroLock/ZeroLock.h"
#include "BaseGameplayAbility.generated.h"

class UImage;
class APredictedProjectile;
class UBaseCharAbilitySystemComponent;
/**
 * 
 */

UENUM(BlueprintType)
enum class EGameplayAbilitySlot : uint8
{
	AbilitySlot1 UMETA(DisplayName = "Ability Slot 1"),
	AbilitySlot2 UMETA(DisplayName = "Ability Slot 2"),
	AbilitySlot3 UMETA(DisplayName = "Ability Slot 3"),
	UltimateSlot UMETA(DisplayName = "Ultimate Slot "),
};
UCLASS()
class ZEROLOCK_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseGameplayAbility();

	void SetSlot(EGameplayAbilitySlot slot);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASTargetConfirmationStyle TargetStyle = EGASTargetConfirmationStyle::Instant;

public:
	UFUNCTION()
	void ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> GEToApply,UAbilitySystemComponent* TargetASC,UAbilitySystemComponent* SourceASC);

	UFUNCTION()
	void ApplyGameplyEffectToSelf(TSubclassOf<UGameplayEffect> GEToApply,UAbilitySystemComponent* SourseASC);

	UPROPERTY()
	APredictedProjectile* PredictedProjectile;
	
	UFUNCTION()
	UAbilitySystemComponent* GetOwnerASC();


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
	TObjectPtr<UTexture2D> IconImage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
	FString AbilityName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Icon")
	FString AbilityDescription;


	UPROPERTY()
	EGameplayAbilitySlot Slot;

	UFUNCTION()
	void SetInputID(EGASAbilityInputID in);
	
};
