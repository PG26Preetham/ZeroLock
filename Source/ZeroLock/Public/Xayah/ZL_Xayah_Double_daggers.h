// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GAS/ZL_BasePlayAnimation_AndDo.h"
#include "ZL_Xayah_Double_daggers.generated.h"

class AZL_Xayah_projectile;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Xayah_Double_daggers : public UZL_BasePlayAnimation_AndDo
{
	GENERATED_BODY()

	virtual void OnAnimationPointTrigger() override;

	void Fire(AZeroLockCharacter* Hero ,FVector Location , FRotator Rotation,FActorSpawnParameters SpawnParm);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AZL_Xayah_projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> FeatherDownClassClass;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
