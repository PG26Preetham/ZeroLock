// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/ZL_VMCR_Attributes.h"

#include "Components/WidgetComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UObject* UZL_VMCR_Attributes::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                             const UMVVMView* View) const
{
	if (!UserWidget) return nullptr;
	
	if (AZeroLockCharacter* MyChar = Cast<AZeroLockCharacter>(UserWidget->GetOwningPlayerPawn()))
	{
		return MyChar->GetVMObject_Attributes();
	}

	return nullptr;
}
