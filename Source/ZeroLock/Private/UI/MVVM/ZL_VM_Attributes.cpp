//Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/ZL_VM_Attributes.h"

#include "ZeroLock/ZeroLock.h"

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



void UZL_VM_Attributes::SetAmmo(const int32& ammo)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Ammo,ammo))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetFinalAmmoText);
	}
}

void UZL_VM_Attributes::SetMaxAmmo(const int32& maxammo)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxAmmo,maxammo))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetFinalAmmoText);
	}
}




void UZL_VM_Attributes::SetIsInfiniteAmmo(const bool& isInfinite)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(IsInfiniteAmmo,isInfinite))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetFinalAmmoText);
	}
}

const FString UZL_VM_Attributes::GetFinalAmmoText() const
{
	FString ammoText = FString::FromInt(Ammo)+"/"+FString::FromInt(MaxAmmo);
	if (IsInfiniteAmmo)
	{
		ammoText = "00";
	}
	return ammoText;
}

void UZL_VM_Attributes::SetVM_ChargePhase(UZL_VM_ChargePercent* inVM)
{

	if (UE_MVVM_SET_PROPERTY_VALUE(VM_ChargePhase,inVM))
	{
		
	}
}
