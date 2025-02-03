// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZeroLockGameMode.h"
#include "ZeroLockCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZeroLockGameMode::AZeroLockGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
