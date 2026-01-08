//Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Abilities/ZL_VM_AbilityIcon.h"

#include "ZeroLock/ZeroLock.h"

void UZL_VM_AbilityIcon::SetIconTexture(UTexture2D* NewIcon)
{
	
	if (UE_MVVM_SET_PROPERTY_VALUE(IconTexture, NewIcon))
	{
		UpdateCooldown(0,1);
	}
}

void UZL_VM_AbilityIcon::UpdateCooldown(float Remaining, float Duration)
{
		float NewPercent = (Duration > 0.f) ? (Remaining / Duration) : 0.f;
		UE_MVVM_SET_PROPERTY_VALUE(CooldownPercent, 1-NewPercent);
		UE_MVVM_SET_PROPERTY_VALUE(bIsOnCooldown, Remaining > 0.f);
		
		int32 Seconds = FMath::CeilToInt(Remaining);
		UE_MVVM_SET_PROPERTY_VALUE(CooldownText, Seconds > 0 ? FText::AsNumber(Seconds) : FText::GetEmpty());
}

void UZL_VM_AbilityIcon::SetbHasStacks(bool bstacks)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(bHasStacks, bstacks))
	{
		
	}
}

void UZL_VM_AbilityIcon::SetStackNum(int32 NewStackNum)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(StackNum, NewStackNum))
	{
		
	}
}
