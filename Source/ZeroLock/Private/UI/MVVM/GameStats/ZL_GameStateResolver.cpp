// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/GameStats/ZL_GameStateResolver.h"

#include "Blueprint/UserWidget.h"
#include "Gamemode/Zero_BaseGameState.h"
#include "UI/MVVM/GameStats/ZL_VM_GameState.h"

UObject* UZL_GameStateResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                               const UMVVMView* View) const
{
	if (AZero_BaseGameState* GS = UserWidget->GetWorld()->GetGameState<AZero_BaseGameState>())
	{
		// Return the VM instance held by the GameState
		return GS->GetGameStateVM();
	}
	return nullptr;
}
