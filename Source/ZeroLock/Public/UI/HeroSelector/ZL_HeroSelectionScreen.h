// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ZL_HeroSelectionScreen.generated.h"

class AZL_CharacterSelector_Actor;
class UImage;
class UZL_Character_Data_Asset;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_HeroSelectionScreen : public UCommonActivatableWidget
{
	GENERATED_BODY()

	UZL_HeroSelectionScreen();
public:
	void HoveredItemChanged(UObject* Object, bool bArg);
	virtual void NativeOnInitialized() override;


	UPROPERTY()
	TArray<UZL_Character_Data_Asset*> LoadedItems;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget))
	class UCommonTileView* HeroSelectionList;


	UPROPERTY(meta = (BindWidget))
	UImage* RenderTarget;

	void LoadItemsAsync();
	void OnItemsLoaded();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	UDataTable* HeroTable;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero")
	AZL_CharacterSelector_Actor* RenderHeroActor;
	
};
