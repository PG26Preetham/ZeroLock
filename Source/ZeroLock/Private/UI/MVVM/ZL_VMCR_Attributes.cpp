//Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/ZL_VMCR_Attributes.h"

#include "Zero_BasePlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/MVVM/ZL_VM_Attributes.h"
#include "ZeroLock/ZeroLockCharacter.h"

UObject* UZL_VMCR_Attributes::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                             const UMVVMView* View) const
{
	if (!UserWidget) return nullptr;
	
	if (AZeroLockCharacter* MyChar = Cast<AZeroLockCharacter>(UserWidget->GetOwningPlayerPawn()))
	{
		return MyChar->GetVM_Attributes();
	}
	
	return nullptr;
}
