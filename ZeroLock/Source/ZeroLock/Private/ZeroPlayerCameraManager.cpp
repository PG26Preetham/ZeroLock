// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroPlayerCameraManager.h"
#include "ZeroBaseCharacterMovementComp.h"
#include "ZeroLock/ZeroLockCharacter.h"
#include "Components/CapsuleComponent.h"

AZeroPlayerCameraManager::AZeroPlayerCameraManager()
{
}

void AZeroPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if(AZeroLockCharacter* ZeroCharacter = Cast<AZeroLockCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UZeroBaseCharacterMovementComp* ZMC = Cast<UZeroBaseCharacterMovementComp>(ZeroCharacter->GetCharacterMovement());
		if(!ZMC)
		{
			return;
		}
		FVector TargetCrouchOffset = FVector(0,0,ZMC->GetCrouchedHalfHeight() - ZeroCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (ZMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
