// Fill out your copyright notice in the Description page of Project Settings.


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
