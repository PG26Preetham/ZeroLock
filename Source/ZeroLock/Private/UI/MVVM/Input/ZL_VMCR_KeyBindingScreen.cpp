// Copyright Preetham Mukundan (C) 2026


#include "UI/MVVM/Input/ZL_VMCR_KeyBindingScreen.h"

#include "Blueprint/UserWidget.h"
#include "UI/MVVM/Input/ZL_VM_KeyBindingList.h"

UObject* UZL_VMCR_KeyBindingScreen::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                                   const UMVVMView* View) const
{
	if (!ExpectedType->IsChildOf(UZL_VM_KeyBindingList::StaticClass()))
	{
		return nullptr;
	}
	
	UZL_VM_KeyBindingList* NewViewModel = NewObject<UZL_VM_KeyBindingList>(const_cast<UUserWidget*>(UserWidget));

	if (UserWidget)
	{
		ULocalPlayer* LocalPlayer = UserWidget->GetOwningLocalPlayer();
		if (LocalPlayer)
		{
			NewViewModel->InitializeWithPlayer(LocalPlayer);
		}
	}

	return NewViewModel;
}
