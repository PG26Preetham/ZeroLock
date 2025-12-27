// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ZL_AbilityUIManagerComponent.generated.h"


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


	
	TMap<FGameplayTag, UZL_VM_AbilityIcon*> AbilityTagMap;

	void InitializeTagMap();
	

	void OnCooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void RefreshCooldowns();

	UPROPERTY()
	UZL_VM_AbilitiesContainer* VM_Abilities;
private:

	TMap<FGameplayTag, UZL_VM_AbilityIcon*> TagToSlotMap;
	FTimerHandle CooldownTimerHandle;
		
};
