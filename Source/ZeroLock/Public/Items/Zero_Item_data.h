//Copyright Preetham Mukundan (C) 2026

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
	Vitality UMETA(DisplayName="VitalityItems")
	
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SellRefundPercent = 0.5f;

	// Passive stat buffs
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> PassiveEffects;

	// Active or passive abilities
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UBaseGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZeroLockItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UZero_Item_data*> NextItemsToUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZeroLockItemTier ItemTierType;
};
