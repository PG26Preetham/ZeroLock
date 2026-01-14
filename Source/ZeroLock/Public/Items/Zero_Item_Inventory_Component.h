//Copyright Preetham Mukundan (C) 2026

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

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FZeroInventoryItem* ItemsUpgradedFrom;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZeroInventoryItemDelegate, TArray<FZeroInventoryItem>, InventoryItem);

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
	int32 MaxSlots = 12;

	/** GAS reference */
	UAbilitySystemComponent* GetASC() const;

	//------------------------------------
	// Buying / Selling
	//------------------------------------

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void ServerBuyItem(UZero_Item_data* ItemData , UZero_Item_data* ItemUpgradedFrom);

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void ServerSellItem(UZero_Item_data* ItemData);

protected:

	void ApplyItemEffects(FZeroInventoryItem& NewItem);
	void RemoveItemEffects(FZeroInventoryItem& Item);

	// Helpers
	bool HasEnoughSouls(int32 Cost) const;
	void DeductSouls(int32 Cost);
	int32 AddItem(const FZeroInventoryItem& Item);

	FZeroInventoryItem* FindItem(UZero_Item_data* ItemToSearch);
	int32 FindItemIndex(UZero_Item_data* ItemToSearch);


public:
	UPROPERTY(BlueprintAssignable)
	FZeroInventoryItemDelegate ItemDelegate;
	
	
	UFUNCTION(BlueprintCallable)
	bool HasSlotToBuy();
		
};
