//Copyright Preetham Mukundan (C) 2026


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
	if (!GetOwner()->HasAuthority()) return;
	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_WeaponClass, 1.f, Context);

	if (!SpecHandle.IsValid())	return;

	//Setting the base damage value 
	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Weapon",false), DamageValue);
	//Apply the effect 
	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);

	//Sending event to the source
	

	FGameplayTag TagToSend =FGameplayTag::RequestGameplayTag("Event.WeaponHit",false);
	SendGameplayEventToSelf(TagToSend, TargetASC);
	TagToSend = FGameplayTag::RequestGameplayTag("Event.WeaponRecieved",false);
	SendGameplayEventToTarget(TagToSend, TargetASC);

	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),FGameplayTag::RequestGameplayTag("Event.WeaponHit",false),WeaponHitEventData);
	
}

void UBaseCharAbilitySystemComponent::ApplySpiritDamage(UAbilitySystemComponent* TargetASC, float DamageValue)
{
	if (!TargetASC || !GE_SpiritClass)return;
	if (!GetOwner()->HasAuthority()) return;
	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_SpiritClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Spirit",false), DamageValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);

	FGameplayTag TagToSend =FGameplayTag::RequestGameplayTag("Event.SpiritHit",false);
	SendGameplayEventToSelf(TagToSend, TargetASC);
	TagToSend = FGameplayTag::RequestGameplayTag("Event.SpiritRecieved",false);
	SendGameplayEventToTarget(TagToSend, TargetASC);
}

void UBaseCharAbilitySystemComponent::ApplyMeleeDamage(UAbilitySystemComponent* TargetASC, float DamageValue)
{
	if (!TargetASC || !GE_MeleeClass)return;
	if (!GetOwner()->HasAuthority()) return;
	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_MeleeClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.DamageCalc.Melee",false), DamageValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);

	FGameplayTag TagToSend =FGameplayTag::RequestGameplayTag("Event.MeleeHit",false);
	SendGameplayEventToSelf(TagToSend, TargetASC);
	TagToSend = FGameplayTag::RequestGameplayTag("Event.MeleeRecieved",false);
	SendGameplayEventToTarget(TagToSend, TargetASC);
}

void UBaseCharAbilitySystemComponent::ApplyHeal(UAbilitySystemComponent* TargetASC, float HealValue)
{
	if (!TargetASC || !GE_HealingClass)return;

	FGameplayEffectContextHandle Context = MakeEffectContext();
	Context.AddSourceObject(GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingSpec(GE_HealingClass, 1.f, Context);

	if (!SpecHandle.IsValid())
		return;

	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Zerolock.HealCalc.Healing",false), HealValue);

	ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);

	FGameplayTag TagToSend =FGameplayTag::RequestGameplayTag("Event.HealHit",false);
	SendGameplayEventToSelf(TagToSend, TargetASC);
	TagToSend = FGameplayTag::RequestGameplayTag("Event.HealRecieved",false);
}

void UBaseCharAbilitySystemComponent::SendGameplayEventToSelf(FGameplayTag Tag, UAbilitySystemComponent* TargetASC)
{

	FGameplayEventData EventDataToSend;
	EventDataToSend.Instigator = GetAvatarActor();
	EventDataToSend.EventTag =Tag;
	EventDataToSend.ContextHandle =MakeEffectContext();
	EventDataToSend.ContextHandle.AddSourceObject(GetAvatarActor());
	EventDataToSend.Instigator= GetAvatarActor();
	EventDataToSend.Target =TargetASC->GetAvatarActor();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),Tag,EventDataToSend);
	
}

void UBaseCharAbilitySystemComponent::SendGameplayEventToTarget(FGameplayTag Tag, UAbilitySystemComponent* TargetASC)
{
	FGameplayEventData EventDataToSend;
	EventDataToSend.Instigator = GetAvatarActor();
	EventDataToSend.EventTag =Tag;
	EventDataToSend.ContextHandle =MakeEffectContext();
	EventDataToSend.ContextHandle.AddSourceObject(GetAvatarActor());
	EventDataToSend.Instigator= GetAvatarActor();
	EventDataToSend.Target =TargetASC->GetAvatarActor();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetASC->GetAvatarActor(),Tag,EventDataToSend);
	
}
