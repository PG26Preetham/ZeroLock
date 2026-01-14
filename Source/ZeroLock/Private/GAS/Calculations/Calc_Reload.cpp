//Copyright Preetham Mukundan (C) 2026


#include "GAS/Calculations/Calc_Reload.h"

#include "GAS/BaseCharAttributeSet.h"

struct Zero_ReloadStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentAmmo);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxAmmo);

	Zero_ReloadStatics()
	{

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,MaxAmmo, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseCharAttributeSet,CurrentAmmo, Target, true);
	}
};
static const Zero_ReloadStatics& ReloadStatics()
{
	static Zero_ReloadStatics ZRStatics;
	return ZRStatics;
}
UCalc_Reload::UCalc_Reload()
{
	RelevantAttributesToCapture.Add(ReloadStatics().CurrentAmmoDef);
	RelevantAttributesToCapture.Add(ReloadStatics().MaxAmmoDef);
}

void UCalc_Reload::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	float MaxAmmo = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ReloadStatics().MaxAmmoDef, EvaluationParameters, MaxAmmo);
	MaxAmmo = FMath::Max<float>(MaxAmmo, 0.0f);

	float CurrentAmmo = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ReloadStatics().CurrentAmmoDef, EvaluationParameters, CurrentAmmo);
	CurrentAmmo = FMath::Max<float>(CurrentAmmo, 0.0f);

	if (MaxAmmo >= 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ReloadStatics().CurrentAmmoProperty, EGameplayModOp::Override, MaxAmmo));
	}
}
