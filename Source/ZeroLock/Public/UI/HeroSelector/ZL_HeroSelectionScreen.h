//Copyright Preetham Mukundan (C) 2026

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

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleOnHoveredChanged(UObject* Item, bool bIsHovered);

	UFUNCTION()
	void HandleOnSelectionChanged(UObject* Item);

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTileView> HeroSelectionList;

	UPROPERTY(BlueprintReadWrite, Category = "MVVM")
	UZl_CharacterSelectionVM* SelectionVM;


	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void VM_PopulateList(TArray<UZL_Character_Data_Asset*> DataArray);

private:

	UZL_CharacterSelectionSubsystem* GetSelectionSubsystem() const;
	
};
