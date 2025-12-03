// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Zero_Item_data.generated.h"

class UGameplayEffect;
class UBaseGameplayAbility;
/**
 * 
 */

UENUM(BlueprintType)
enum class EZeroLockItemType : uint8
{
	Weapon UMETA(DisplayName="WeaponItems"),
	Spirit UMETA(DisplayName="SpiritItems"),
	Health UMETA(DisplayName="HealthItems")
	
	
};
UENUM(BlueprintType)
enum class EZeroLockItemTier : uint8
{
	Tier1 UMETA(DisplayName="Tier1"),
	Tier2 UMETA(DisplayName="Tier2"),
	Tier3 UMETA(DisplayName="Tier3"),
	Tier4 UMETA(DisplayName="Tier4")
};

UCLASS()
class ZEROLOCK_API UZero_Item_data : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Cost = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SellRefundPercent = 0.5f;

	// Passive stat buffs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PassiveEffects;

	// Active or passive abilities
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UBaseGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EZeroLockItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UZero_Item_data*> NextItemsToUpgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EZeroLockItemTier ItemTierType;
};
