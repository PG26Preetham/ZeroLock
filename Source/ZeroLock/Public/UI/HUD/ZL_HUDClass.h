// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZL_HUDClass.generated.h"

/**
 * 
 */
 class UZL_BaseUILayout;
class UCommonActivatableWidget;
UCLASS()
class ZEROLOCK_API AZL_HUDClass : public AHUD
{
	GENERATED_BODY()
	
	public:
    	UPROPERTY(EditDefaultsOnly)
    	TSubclassOf<UZL_BaseUILayout> BaseUILayer;
    
    	UPROPERTY(BlueprintReadWrite, Category="RootUILayer")
    	UZL_BaseUILayout* RootLayer;
    
    	UFUNCTION(BlueprintCallable)
    	UZL_BaseUILayout* ReturnRootLayer(); 
    
    	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> PlayerInGameHUD;

protected:

	UFUNCTION(BlueprintCallable)
	void PushHUD();
	
};
