// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseComp/MyZL_BasePlayerHealthBar.h"

#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLockCharacter.h"


void UMyZL_BasePlayerHealthBar::AddToDelegate()
{
	Super::AddToDelegate();

	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		Hero->HealthChangeDelegate.AddUniqueDynamic(this,&ThisClass::OnBarValueChanged);
	}
}

void UMyZL_BasePlayerHealthBar::OnBarValueChanged(float NewValue, float MaxValue)
{
	Super::OnBarValueChanged(NewValue, MaxValue);
	ZLOG("HealthChanged");
}
