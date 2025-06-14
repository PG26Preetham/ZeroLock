// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculations/Calc_WeaponDamage.h"
#include "GAS/BaseCharAttributeSet.h"


struct Zero_WeaponDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(WeaponDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WeaponResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	Zero_WeaponDamageStatics()
	{

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,WeaponDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,Damage, Source, true);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet, WeaponResistance, Target, false);
	}
};
static const Zero_WeaponDamageStatics& WeaponDamageStatics()
{
	static Zero_WeaponDamageStatics ZWStatics;
	return ZWStatics;
}
UCalc_WeaponDamage::UCalc_WeaponDamage()
{
	RelevantAttributesToCapture.Add(WeaponDamageStatics().WeaponDamageDef);
	RelevantAttributesToCapture.Add(WeaponDamageStatics().WeaponResistanceDef);
	RelevantAttributesToCapture.Add(WeaponDamageStatics().DamageDef);
}

void UCalc_WeaponDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float WeaponResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(WeaponDamageStatics().WeaponResistanceDef, EvaluationParameters, WeaponResistance);
	WeaponResistance = FMath::Max<float>(WeaponResistance, 0.0f);

	float WeaponDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(WeaponDamageStatics().WeaponDamageDef, EvaluationParameters, WeaponDamage);
	WeaponDamage = FMath::Max<float>(WeaponDamage, 0.0f);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(WeaponDamageStatics().DamageDef, EvaluationParameters, Damage);

	
	float UnmitigatedDamage = Damage + (Damage * WeaponDamage/100); // Can multiply any damage boosters here
	
	float MitigatedDamage = (UnmitigatedDamage) * (1- (WeaponResistance/100));

	if (MitigatedDamage >= 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(WeaponDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
