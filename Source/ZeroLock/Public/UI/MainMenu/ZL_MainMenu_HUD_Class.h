// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/ZL_HUDClass.h"
#include "ZL_MainMenu_HUD_Class.generated.h"

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_MainMenu_HUD_Class : public AZL_HUDClass
{
	GENERATED_BODY()

	public:
	virtual void PushHUD() override;
	
};
