// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Zero_Item_Inventory_Component.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "Items/Zero_Item_data.h"
#include "Net/UnrealNetwork.h"
#include "ZeroLock/ZeroLockCharacter.h"


UZero_Item_Inventory_Component::UZero_Item_Inventory_Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UZero_Item_Inventory_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UZero_Item_Inventory_Component::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UZero_Item_Inventory_Component, Items);
}

UAbilitySystemComponent* UZero_Item_Inventory_Component::GetASC() const
{
	AActor* Owner = GetOwner();
	return Owner ? Owner->FindComponentByClass<UAbilitySystemComponent>() : nullptr;
}

void UZero_Item_Inventory_Component::ServerBuyItem_Implementation(UZero_Item_data* ItemData)
{
	if (!ItemData) return;
	if (!HasEnoughSouls(ItemData->Cost)) return;

	// Deduct souls
	DeductSouls(ItemData->Cost);

	// Add item
	FZeroInventoryItem NewItem;
	NewItem.ItemData = ItemData;

	int32 Index = AddItem(NewItem);
	if (Index == -1) return;
	ZLOG("Item bought");
	ZLOG(ItemData->ItemName);
	ApplyItemEffects(Items[Index]);
}

void UZero_Item_Inventory_Component::ServerSellItem_Implementation(UZero_Item_data* ItemData)
{
	if (!ItemData) return;
	ZLOG("Found item to delete");
	int32 FoundIndex = Items.IndexOfByPredicate(
		[&](const FZeroInventoryItem& Item)
		{
			return Item.ItemData == ItemData;
		}
	);

	if (FoundIndex == INDEX_NONE) return;

	ZLOG("Found item to delete");
	FZeroInventoryItem& Item = Items[FoundIndex];

	int32 Refund = Item.ItemData->Cost * 0.5f;

	DeductSouls(Refund); // Add back souls
	RemoveItemEffects(Item);

	Items.RemoveAt(FoundIndex);
}

void UZero_Item_Inventory_Component::ApplyItemEffects(FZeroInventoryItem& Item)
{
	
	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC || !Item.ItemData) return;
	// Passive Stat Effect
	if (Item.ItemData->PassiveEffects)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Item.ItemData->PassiveEffects, 0, Context);
		Item.StatEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}

	// Passive & Active Abilities
	TArray<TSubclassOf<UBaseGameplayAbility>> Abilities =Item.ItemData->GrantedAbilities;
	


	for (TSubclassOf<UBaseGameplayAbility> Ability : Abilities)
	{
		if (!Ability) continue;
		EGASAbilityInputID AbiltyInputID = EGASAbilityInputID::None;
		FGameplayAbilitySpecHandle GrantedHandle;
		FGameplayAbilitySpec GrantedSpec=FGameplayAbilitySpec(Ability, 1, static_cast<int32>(AbiltyInputID), this);
		
		
		if (Ability.GetDefaultObject()->TargetStyle == EGASTargetConfirmationStyle::Passive)
		{
			EGASAbilityInputID AbiltyInputIDX = EGASAbilityInputID::None;
			GrantedSpec.InputID = static_cast<int32>(AbiltyInputIDX);
			GrantedHandle =ASC->GiveAbility(GrantedSpec);
			//DefaultAbilitiesHandles.Add(GrantedHandle);
			
		}
		else
		{
			GrantedHandle = ASC->GiveAbility(GrantedSpec);
		}
		
		Item.AbilitySpecHandles.Add(GrantedHandle);
	}
}

void UZero_Item_Inventory_Component::RemoveItemEffects(FZeroInventoryItem& Item)
{
	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC) return;

	// Remove stat effect
	if (Item.StatEffectHandle.IsValid())
	{
		ZLOG("removing Gameplay effect");
		ASC->RemoveActiveGameplayEffect(Item.StatEffectHandle);
	}

	// Remove abilities
	for (auto& Handle : Item.AbilitySpecHandles)
	{
		ZLOG("removing ability");
		ASC->ClearAbility(Handle);
	}

	Item.AbilitySpecHandles.Empty();
}

bool UZero_Item_Inventory_Component::HasEnoughSouls(int32 Cost) const
{
	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC) return false;
	AZeroLockCharacter* hero = Cast<AZeroLockCharacter>(GetOwner());
	if (!hero) return false;
    return 	 hero->GetMyAttributeSet()->GetSoul() >= Cost;

}

void UZero_Item_Inventory_Component::DeductSouls(int32 Cost)
{
	if (!ItemCostGameplayEffect) return;

	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC) return;

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ItemCostGameplayEffect, 1, ASC->MakeEffectContext());
	if (Spec.Data)
	{
		Spec.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag("Zerolock.Item.Cost",false),
			-Cost
		);

		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
	}
}

int32 UZero_Item_Inventory_Component::AddItem(const FZeroInventoryItem& Item)
{
	if (Items.Num() >= MaxSlots)
		return -1;

	int32 Index = Items.Add(Item);
	return Index;
}
