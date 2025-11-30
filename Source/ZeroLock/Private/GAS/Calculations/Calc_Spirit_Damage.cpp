// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculations/Calc_Spirit_Damage.h"

#include "GAS/BaseCharAttributeSet.h"


struct Zero_SpiritDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(SpiritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SpiritResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	Zero_SpiritDamageStatics()
	{

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,SpiritDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,Damage, Source, true);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet, SpiritResistance, Target, false);
	}
};
static const Zero_SpiritDamageStatics& SpiritDamageStatics()
{
	static Zero_SpiritDamageStatics ZSStatics;
	return ZSStatics;
}
UCalc_Spirit_Damage::UCalc_Spirit_Damage()
{
	RelevantAttributesToCapture.Add(SpiritDamageStatics().SpiritDamageDef);
	RelevantAttributesToCapture.Add(SpiritDamageStatics().SpiritResistanceDef);
	RelevantAttributesToCapture.Add(SpiritDamageStatics().DamageDef);
}

void UCalc_Spirit_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	float SpiritResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SpiritDamageStatics().SpiritResistanceDef, EvaluationParameters, SpiritResistance);
	SpiritResistance = FMath::Max<float>(SpiritResistance, 0.0f);

	float SpiritDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SpiritDamageStatics().SpiritDamageDef, EvaluationParameters, SpiritDamage);
	SpiritDamage = FMath::Max<float>(SpiritDamage, 0.0f);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SpiritDamageStatics().DamageDef, EvaluationParameters, Damage);

	
	float UnmitigatedDamage = Damage + (Damage * SpiritDamage/100); // Can multiply any damage boosters here
	
	float MitigatedDamage = (UnmitigatedDamage) * (1- (SpiritResistance/100));

	if (MitigatedDamage >= 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(SpiritDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
