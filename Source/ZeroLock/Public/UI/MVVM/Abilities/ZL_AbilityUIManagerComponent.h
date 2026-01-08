//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "ZL_AbilityUIManagerComponent.generated.h"


class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpec;
class UZL_VM_AbilitiesContainer;
class UZL_VM_AbilityIcon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROLOCK_API UZL_AbilityUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZL_AbilityUIManagerComponent();

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	UObject* GetAbilitiesViewModel() ;

	UFUNCTION()
	void OnAbilityAdded(FGameplayAbilitySpec& Spec);
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<FGameplayTag, UZL_VM_AbilityIcon*> StackTagToSlotMap;

	// Callback for when stack tags change
	void OnStackTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY()
	TMap<FGameplayTag, UZL_VM_AbilityIcon*> AbilityTagMap;

	void InitializeTagMap();


	void OnStackChanged(FActiveGameplayEffectHandle Handle, int32 NewCount, int32 OldCount);


	void OnGEApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);


	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UZL_VM_AbilityIcon*> ActiveHandleToSlotMap;

	void OnCooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void RefreshCooldowns();

	UPROPERTY()
	UZL_VM_AbilitiesContainer* VM_Abilities;
private:

	TMap<FGameplayTag, UZL_VM_AbilityIcon*> TagToSlotMap;
	FTimerHandle CooldownTimerHandle;
		
};
