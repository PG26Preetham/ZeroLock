// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/ZL_VM_Attributes.h"

void UZL_VM_Attributes::SetHealth(const float& health)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Health,health))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}

void UZL_VM_Attributes::SetMaxHealth(const float& maxhealth)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth,maxhealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
	}
}
