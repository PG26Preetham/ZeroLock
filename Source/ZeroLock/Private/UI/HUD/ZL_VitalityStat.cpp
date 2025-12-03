// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_VitalityStat.h"

#include "GAS/BaseCharAbilitySystemComponent.h"
#include "GAS/BaseCharAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BaseComp/ZL_Stat_Box.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_VitalityStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetupDelegates();
}

void UZL_VitalityStat::SetupDelegates()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		HealingBonus->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetHealingBonusAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		HealingReduction->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetHealingReductionAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		HealthRegen->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetHealthRegenerationAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());
		MaxHealth->InitGameplayAttribute(Hero->GetMyAttributeSet()->GetMaximumHealthAttribute(),Hero->GetAbilitySystemComponent(),Hero->GetMyAttributeSet());

		
	}
}
