// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/BaseCharAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

void UBaseCharAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (OnNewAbilityAdded.IsBound())
	{
		OnNewAbilityAdded.Broadcast(AbilitySpec);
	}
	
}

void UBaseCharAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		OnNewAbilityAdded.Broadcast(Spec);
	}
}

void UBaseCharAbilitySystemComponent::ApplyWeaponDamage(UAbilitySystemComponent* TargetASC, float DamageValue)
{
	if (!TargetASC || !GE_WeaponClass)return;

	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_WeaponClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Weapon",false), DamageValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);

	FGameplayEventData WeaponHitEventData;
	WeaponHitEventData.EventTag =FGameplayTag::RequestGameplayTag("Event.WeaponHit",false);
	WeaponHitEventData.ContextHandle =MakeEffectContext();
	WeaponHitEventData.ContextHandle.AddSourceObject(GetAvatarActor());
	WeaponHitEventData.Instigator= GetAvatarActor();
	WeaponHitEventData.Target =TargetASC->GetAvatarActor();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),FGameplayTag::RequestGameplayTag("Event.WeaponHit",false),WeaponHitEventData);
	
}

void UBaseCharAbilitySystemComponent::ApplySpiritDamage(UAbilitySystemComponent* TargetASC, float DamageValue)
{
	if (!TargetASC || !GE_SpiritClass)return;

	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_SpiritClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Spirit",false), DamageValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
}

void UBaseCharAbilitySystemComponent::ApplyMeleeDamage(UAbilitySystemComponent* TargetASC, float DamageValue)
{
	if (!TargetASC || !GE_MeleeClass)return;

	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_MeleeClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Melee",false), DamageValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
}
