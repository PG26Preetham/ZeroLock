// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseCharAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ZEROLOCK_API UBaseCharAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
		
public:

	UBaseCharAttributeSet();

	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Used to create a local copy of Damage which is then subtracted from Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, Damage)

		// Used to create a local copy of Healing which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, Healing)

		// Holds the current value for Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, CurrentHealth)

		// Holds the value for Maximum Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_MaximumHealth)
	FGameplayAttributeData MaximumHealth;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, MaximumHealth)

		// Holds the value for Health Regeneration.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, HealthRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentSpeed)
	FGameplayAttributeData CurrentSpeed;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, CurrentSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentJump)
	FGameplayAttributeData CurrentJump;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, CurrentJump)

	UPROPERTY(BlueprintReadOnly, Category = "Soul Attribute Set", ReplicatedUsing = OnRep_Souls)
	FGameplayAttributeData Soul;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, Soul)

	UPROPERTY(BlueprintReadOnly,Category = "Weapon Attribute Set", ReplicatedUsing= OnRep_FireRate)
	FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, FireRate)

	UPROPERTY(BlueprintReadOnly,Category = "Weapon Attribute Set", ReplicatedUsing= OnRep_WeaponDamage)
	FGameplayAttributeData WeaponDamage;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, WeaponDamage)

	UPROPERTY(BlueprintReadOnly,Category = "Weapon Attribute Set", ReplicatedUsing= OnRep_WeaponResistance)
	FGameplayAttributeData WeaponResistance;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, WeaponResistance)

	
	UPROPERTY(BlueprintReadOnly,Category = "Weapon Attribute Set", ReplicatedUsing= OnRep_CurrentAmmo)
	FGameplayAttributeData CurrentAmmo;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, CurrentAmmo)

	
	UPROPERTY(BlueprintReadOnly,Category = "Weapon Attribute Set", ReplicatedUsing= OnRep_MaxAmmo)
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, MaxAmmo)

	UPROPERTY(BlueprintReadOnly,Category = "Spirit Attribute Set", ReplicatedUsing= OnRep_SpiritDamage)
	FGameplayAttributeData SpiritDamage;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, SpiritDamage)

	UPROPERTY(BlueprintReadOnly,Category = "Spirit Attribute Set", ReplicatedUsing= OnRep_SpiritResistance)
	FGameplayAttributeData SpiritResistance;
	ATTRIBUTE_ACCESSORS(UBaseCharAttributeSet, SpiritResistance)
	

protected:

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;

	UFUNCTION()
		virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_CurrentSpeed(const FGameplayAttributeData& OldValue);


	UFUNCTION()
		virtual void OnRep_CurrentJump(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaximumHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Souls(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_FireRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_WeaponDamage(const FGameplayAttributeData& OldValue);

	
	UFUNCTION()
	virtual void OnRep_CurrentAmmo(const FGameplayAttributeData& OldValue);

	
	UFUNCTION()
	virtual void OnRep_MaxAmmo(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_WeaponResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_SpiritDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_SpiritResistance(const FGameplayAttributeData& OldValue);
	
};
