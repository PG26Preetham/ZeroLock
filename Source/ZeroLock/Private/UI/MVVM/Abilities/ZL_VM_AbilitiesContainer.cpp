// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/Abilities/ZL_VM_AbilitiesContainer.h"

#include "UI/MVVM/Abilities/ZL_VM_AbilityIcon.h"
#include "ZeroLock/ZeroLock.h"

UZL_VM_AbilitiesContainer::UZL_VM_AbilitiesContainer()
{
		
}

void UZL_VM_AbilitiesContainer::InitSlots()
{
	if (!Slot_Secondary)
	{
		SetSlot_Secondary(NewObject<UZL_VM_AbilityIcon>(this));
	}
	if (!Slot_Ability1)
	{
		SetSlot_Ability1(NewObject<UZL_VM_AbilityIcon>(this));
	}
	if (!Slot_Ability2)
	{
		SetSlot_Ability2(NewObject<UZL_VM_AbilityIcon>(this));
	}
	if (!Slot_Ultimate)
	{
		SetSlot_Ultimate(NewObject<UZL_VM_AbilityIcon>(this));
	}
	
}

void UZL_VM_AbilitiesContainer::SetSlot_Secondary(UZL_VM_AbilityIcon* iconSlotVM)
{
	
	UE_MVVM_SET_PROPERTY_VALUE(Slot_Secondary, iconSlotVM);
}

void UZL_VM_AbilitiesContainer::SetSlot_Ability1(UZL_VM_AbilityIcon* iconSlotVM)
{

	UE_MVVM_SET_PROPERTY_VALUE(Slot_Ability1, iconSlotVM);
}

void UZL_VM_AbilitiesContainer::SetSlot_Ability2(UZL_VM_AbilityIcon* iconSlotVM)
{

	UE_MVVM_SET_PROPERTY_VALUE(Slot_Ability2, iconSlotVM);
}

void UZL_VM_AbilitiesContainer::SetSlot_Ultimate(UZL_VM_AbilityIcon* iconSlotVM)
{

	UE_MVVM_SET_PROPERTY_VALUE(Slot_Ultimate, iconSlotVM);
}
