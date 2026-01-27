//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ZeroLock/ZeroLock.h"
#include "Zero_BasePlayerStart.generated.h"

class UZL_VM_Attributes;

UCLASS()
class ZEROLOCK_API AZero_BasePlayerStart : public APlayerStart
{
	GENERATED_BODY()



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeamID TeamID;




};
