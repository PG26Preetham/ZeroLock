// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_WeaponStat.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BaseComp/ZL_Stat_Box.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_WeaponStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetupDelegates();
}

void UZL_WeaponStat::SetupDelegates()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		WeaponDamageBonus->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetWeaponDamageAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		WeaponLifeSteal->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetWeaponLifeStealAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		FlatWeaponDamage->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetFlatWeaponAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());

		MaxAmmo->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetMaxAmmoAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		Firerate->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetFireRateAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());

		WeaponResistance->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetWeaponResistanceAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		WeaponResistanceReduction->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetWeaponResistanceReductionAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
			
	}
}
