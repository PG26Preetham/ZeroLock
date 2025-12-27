// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZL_CharacterSelectionSubsystem.generated.h"

class UZl_CharacterSelectionVM;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_CharacterSelectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Call this from your Initial Loading Screen or Main Menu
	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	void LoadCharacterTable(UDataTable* Table);

	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	TObjectPtr<UZl_CharacterSelectionVM> SelectionVM;

private:
	void OnAssetsLoaded(TArray<FSoftObjectPath> LoadedPaths);
    
	UPROPERTY()
	UDataTable* InternalTable;
	
};
