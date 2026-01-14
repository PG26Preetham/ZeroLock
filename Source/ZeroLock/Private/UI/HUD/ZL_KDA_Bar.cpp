//Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_KDA_Bar.h"

#include "Zero_BasePlayerController.h"
#include "Zero_BasePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BaseComp/ZL_Stat_Box.h"
#include "ZeroLock/ZeroLockCharacter.h"

void UZL_KDA_Bar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AddDelegates();
}

void UZL_KDA_Bar::NewValueChanged(int NewStatValue)
{
	KillStatBox->NewValueChanged(NewStatValue);
}

void UZL_KDA_Bar::AddPS_Delegates(AZero_BasePlayerState* PS)
{
	if (AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OnPSInit.RemoveDynamic(this,&UZL_KDA_Bar::AddPS_Delegates);
	}
	
	if (PS)
	{
		PS->OnKillsChanged.AddUniqueDynamic(KillStatBox,&UZL_Stat_Box::NewValueChanged);
		//PS->OnKillsChanged.AddUniqueDynamic(this,&ThisClass::NewValueChanged);
		PS->OnDeathsChanged.AddUniqueDynamic(DeathStatBox,&UZL_Stat_Box::NewValueChanged);
		PS->OnAssistsChanged.AddUniqueDynamic(AssistStatBox,&UZL_Stat_Box::NewValueChanged);
		KillStatBox->NewValueChanged(PS->Kills);
		DeathStatBox->NewValueChanged(PS->Deaths);
		AssistStatBox->NewValueChanged(PS->Assists);
	}
}

void UZL_KDA_Bar::AddDelegates()
{
	
	if (AZero_BasePlayerController* PC = Cast<AZero_BasePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (PC->HasAuthority())
		{
			AddPS_Delegates(PC->GetPlayerState<AZero_BasePlayerState>());
		}
		PC->OnPSInit.AddUniqueDynamic(this,&UZL_KDA_Bar::AddPS_Delegates);
	}
}
