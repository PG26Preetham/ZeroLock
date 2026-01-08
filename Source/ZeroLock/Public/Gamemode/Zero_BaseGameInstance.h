//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include"OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Zero_BaseGameInstance.generated.h"



UCLASS()
class ZEROLOCK_API UZero_BaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
	TObjectPtr<UDataTable> CharacterDataTable;
	
	virtual void Init() override;
	

};
