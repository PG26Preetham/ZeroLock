//Copyright Preetham Mukundan (C) 2026


#include "UI/HUD/ZL_HUD_PlayerBase.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "UI/HUD/ZL_HUD_CrosshairBase.h"

UZL_HUD_PlayerBase::UZL_HUD_PlayerBase()
{
}

void UZL_HUD_PlayerBase::SetCrosshair()
{
	if (CrosshairSwitcher)
	{
		if (GunCrosshair)
		{
			CrosshairSwitcher->SetActiveWidget(GunCrosshair);
		}
	}
}

void UZL_HUD_PlayerBase::AddToDelegate()
{
}

void UZL_HUD_PlayerBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetCrosshair();
}
