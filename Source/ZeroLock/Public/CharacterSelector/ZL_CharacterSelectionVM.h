// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ZL_CharacterSelectionVM.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterChanged, UZL_Character_Data_Asset*, NewCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCMousePosChanged, FVector2D, newMousePos);
class UZL_Character_Data_Asset;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZl_CharacterSelectionVM : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, Category = "State")
	TObjectPtr<UZL_Character_Data_Asset> CurrentCharacter;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, Category = "State")
	FVector2D LocalMousePos;
	
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, Category = "State")
	TArray<TObjectPtr<UZL_Character_Data_Asset>> CharacterList;

	void SetCurrentCharacter(UZL_Character_Data_Asset* InCharacter) ;
	UZL_Character_Data_Asset* GetCurrentCharacter() const { return CurrentCharacter; }

	void SetCharacterList(TArray<TObjectPtr<UZL_Character_Data_Asset>> InList);
	TArray<TObjectPtr<UZL_Character_Data_Asset>> GetCharacterList() const { return CharacterList; }


	void SetLocalMousePos(FVector2D InLocalMousePos);
	FVector2D GetLocalMousePos() const { return LocalMousePos; }


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterChanged OnCharacterChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCMousePosChanged OnMousePosChanged;
};
