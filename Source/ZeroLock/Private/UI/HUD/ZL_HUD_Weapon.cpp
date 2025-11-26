// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_HUD_Weapon.h"

#include "CommonTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroLock/ZeroLockCharacter.h"


void UZL_HUD_Weapon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AddDelegates();
}

void UZL_HUD_Weapon::OnValueChanged(float currentAmmo, float MaxAmmo)
{
	FString AmmoText = FString::FromInt((int)currentAmmo);
	FString MaxAmmoText = "/" + FString::FromInt((int)MaxAmmo);
	AmmoDisplayText->SetText(FText::FromString(AmmoText));
	MaxAmmoDisplayText->SetText(FText::FromString(MaxAmmoText));
}

void UZL_HUD_Weapon::AddDelegates()
{
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (Hero)
	{
		Hero->AmmoChangeDelegate.AddUniqueDynamic(this,&UZL_HUD_Weapon::OnValueChanged);
	}
}
