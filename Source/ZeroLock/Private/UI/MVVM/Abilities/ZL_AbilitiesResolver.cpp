// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/Abilities/ZL_AbilitiesResolver.h"

#include "Blueprint/UserWidget.h"
#include "UI/MVVM/Abilities/ZL_AbilityUIManagerComponent.h"
#include "ZeroLock/ZeroLockCharacter.h"

UObject* UZL_AbilitiesResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                               const UMVVMView* View) const
{
	if (!UserWidget) return nullptr;
	
	if (AZeroLockCharacter* MyChar = Cast<AZeroLockCharacter>(UserWidget->GetOwningPlayerPawn()))
	{
		// Find the UI Manager Component we created earlier
		if (MyChar->GetAbilityUIManager())
		{
			ZLOG("Getting the vm");
			
			return MyChar->GetAbilityUIManager()->GetAbilitiesViewModel();
		}
	}
	return nullptr;
}
