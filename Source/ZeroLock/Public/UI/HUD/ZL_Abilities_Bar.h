//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GAS/BaseGameplayAbility.h"
#include "ZL_Abilities_Bar.generated.h"


class UZL_VM_AbilitiesContainer;
struct FGameplayAbilitySpec;
class UCommonTextBlock;
class AZeroLockCharacter;
class UZL_HUD_AbilityIcon;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Abilities_Bar : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MVVM")
	void InitializeBar(UZL_VM_AbilitiesContainer* InContainer);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "MVVM")
	UZL_VM_AbilitiesContainer* VM_AbilitiesContainer;
	
};
