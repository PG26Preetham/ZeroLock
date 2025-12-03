// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_SpiritStat.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BaseComp/ZL_Stat_Box.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_SpiritStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetupDelegates();
}

void UZL_SpiritStat::SetupDelegates()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		SpiritDamageBonus->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetSpiritDamageAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		FlatSpiritDamage->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetFlatSpiritAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		SpiritLifeSteal->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetSpiritLifeStealAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		SpiritResistance->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetSpiritResistanceAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		SpiritResistanceReduction->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetSpiritResistanceReductionAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		
		
	}
}
