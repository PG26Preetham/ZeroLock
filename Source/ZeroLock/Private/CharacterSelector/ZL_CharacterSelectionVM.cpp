//Copyright Preetham Mukundan (C) 2026


#include "CharacterSelector/ZL_CharacterSelectionVM.h"

#include "ZeroLock/ZeroLock.h"

void UZl_CharacterSelectionVM::SetCurrentCharacter(UZL_Character_Data_Asset* InCharacter)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentCharacter, InCharacter))
	{
		OnCharacterChanged.Broadcast(InCharacter);
	}
}

void UZl_CharacterSelectionVM::SetCharacterList(TArray<TObjectPtr<UZL_Character_Data_Asset>> InList)
{
	ZLOG("SetCharacterList");
	UE_MVVM_SET_PROPERTY_VALUE(CharacterList, InList); 
}

void UZl_CharacterSelectionVM::SetLocalMousePos(FVector2D InLocalMousePos)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(LocalMousePos, InLocalMousePos))
	{
		OnMousePosChanged.Broadcast(InLocalMousePos);
	}
}

void UZl_CharacterSelectionVM::SetColorSelected(FColor InColorSelected)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(ColorSelected, InColorSelected))
	{
		
	}
}

void UZl_CharacterSelectionVM::SetAbility1(UTexture2D* InAbility1)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Ability1, InAbility1))
	{
		
	}
}

void UZl_CharacterSelectionVM::SetAbility2(UTexture2D* InAbility2)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Ability2, InAbility2))
	{
		
	}
}

void UZl_CharacterSelectionVM::SetAbility3(UTexture2D* InAbility3)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Ability3, InAbility3))
	{
		
	}
}

void UZl_CharacterSelectionVM::SetAbility4(UTexture2D* InAbility4)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Ability4, InAbility4))
	{
		
	}
}

void UZl_CharacterSelectionVM::SetCharacterName(FText InCharacterName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CharacterName, InCharacterName))
	{
		
	}
}
