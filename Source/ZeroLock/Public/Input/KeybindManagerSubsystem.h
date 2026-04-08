// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "KeybindManagerSubsystem.generated.h"

class UInputAction;
class UInputMappingContext;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeybindsUpdated);

USTRUCT(BlueprintType)
struct FKeybindData
{
	GENERATED_BODY()

	UPROPERTY()
	const UInputAction* Action = nullptr;

	UPROPERTY()
	FName MappingName; 

	UPROPERTY()
	FText ActionName;

	UPROPERTY()
	FKey CurrentKey;
};
UCLASS()
class ZEROLOCK_API UKeybindManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnKeybindsUpdated OnKeybindsUpdated;

	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void InitializeKeybinds(UInputMappingContext* BaseIMC);


	UFUNCTION(BlueprintCallable, Category = "Keybindings")
	void RemapKey(FName MappingName, FKey NewKey);

	UFUNCTION(BlueprintPure, Category = "Keybindings")
	TArray<FKeybindData> GetCurrentKeybinds() const { return CurrentKeybinds; }

private:
	UPROPERTY()
	TArray<FKeybindData> CurrentKeybinds;
    
	UPROPERTY()
	UInputMappingContext* CachedBaseIMC;

	void RefreshKeybindData();
	
};
