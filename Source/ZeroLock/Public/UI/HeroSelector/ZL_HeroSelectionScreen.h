// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HeroSelectionScreen.generated.h"

class UCommonTileView;
class UZl_CharacterSelectionVM;
class UZL_Character_Data_Asset;
class UZL_CharacterSelectionSubsystem;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HeroSelectionScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UZL_HeroSelectionScreen();

protected:
	// This is the MVVM/CommonUI equivalent of BeginPlay for Widgets
	virtual void NativeOnInitialized() override;
    
	// Logic for when an icon is hovered in the TileView
	UFUNCTION()
	void HandleOnHoveredChanged(UObject* Item, bool bIsHovered);

	// Logic for when an icon is clicked/selected
	UFUNCTION()
	void HandleOnSelectionChanged(UObject* Item);

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	/** The Grid/List that holds our character icons */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTileView> HeroSelectionList;

	/** Reference to the ViewModel (set manually in C++ or via Subsystem) */
	UPROPERTY(BlueprintReadWrite, Category = "MVVM")
	UZl_CharacterSelectionVM* SelectionVM;


	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void VM_PopulateList(TArray<UZL_Character_Data_Asset*> DataArray);

private:
	/** Helper to get the subsystem easily */
	UZL_CharacterSelectionSubsystem* GetSelectionSubsystem() const;
	
};
