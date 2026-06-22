// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "ZL_VMCR_KeyBindingScreen.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_VMCR_KeyBindingScreen : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()
	
protected:
	
	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;
};
