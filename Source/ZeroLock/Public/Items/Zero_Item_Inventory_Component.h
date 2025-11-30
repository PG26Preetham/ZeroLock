// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "Zero_Item_Inventory_Component.generated.h"

class UGameplayEffect;
class UZero_Item_data;

USTRUCT(BlueprintType)
struct FZeroInventoryItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UZero_Item_data* ItemData = nullptr;

	/** Handle of applied stat effect (if any) */
	UPROPERTY()
	FActiveGameplayEffectHandle StatEffectHandle;

	/** Ability handles we grant */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROLOCK_API UZero_Item_Inventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZero_Item_Inventory_Component();
protected:
	virtual void BeginPlay() override;

public:
virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/** Replicated list of items */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FZeroInventoryItem> Items;

	/** The cost GE we use to deduct/set souls */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ItemCostGameplayEffect;

	/** Max inventory slots */
	UPROPERTY(EditDefaultsOnly)
	int32 MaxSlots = 6;

	/** GAS reference */
	UAbilitySystemComponent* GetASC() const;

	//------------------------------------
	// Buying / Selling
	//------------------------------------

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void ServerBuyItem(UZero_Item_data* ItemData);

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void ServerSellItem(UZero_Item_data* ItemData);

protected:

	void ApplyItemEffects(FZeroInventoryItem& NewItem);
	void RemoveItemEffects(FZeroInventoryItem& Item);

	// Helpers
	bool HasEnoughSouls(int32 Cost) const;
	void DeductSouls(int32 Cost);
	int32 AddItem(const FZeroInventoryItem& Item);
		
};
