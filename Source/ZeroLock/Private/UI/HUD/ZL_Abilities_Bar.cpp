// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ZL_Abilities_Bar.h"
#include "UI/MVVM/Abilities/ZL_VM_AbilitiesContainer.h"



void UZL_Abilities_Bar::InitializeBar(UZL_VM_AbilitiesContainer* InContainer)
{
	VM_AbilitiesContainer = InContainer;
}
