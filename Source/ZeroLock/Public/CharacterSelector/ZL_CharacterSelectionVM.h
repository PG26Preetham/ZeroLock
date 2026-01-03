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
	FColor ColorSelected;
	
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


	FColor GetColorSelected() const { return ColorSelected; }
	void SetColorSelected(FColor InColorSelected);
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter, Category = "State")
	UTexture2D* Ability1;
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter, Category = "State")
	UTexture2D* Ability2;
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter, Category = "State")
	UTexture2D* Ability3;
	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter, Category = "State")
	UTexture2D* Ability4;

	void SetAbility1(UTexture2D* InAbility1);
	void SetAbility2(UTexture2D* InAbility2);
	void SetAbility3(UTexture2D* InAbility3);
	void SetAbility4(UTexture2D* InAbility4);

	UTexture2D* GetAbility1() const { return Ability1; }
	UTexture2D* GetAbility2() const { return Ability2; }
	UTexture2D* GetAbility3() const { return Ability3; }
	UTexture2D* GetAbility4() const { return Ability4; }

	UPROPERTY(BlueprintReadWrite,FieldNotify,Setter,Getter, Category = "State")
	FText CharacterName;

	void SetCharacterName(FText InCharacterName);

	FText GetCharacterName() const { return CharacterName; }
};
