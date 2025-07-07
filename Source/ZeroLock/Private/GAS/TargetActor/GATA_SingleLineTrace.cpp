// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/GATA_SingleLineTrace.h"

#include "Abilities/GameplayAbility.h"
#include "ZeroLock/ZeroLockCharacter.h"

AGATA_SingleLineTrace::AGATA_SingleLineTrace()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bDestroyOnConfirmation = true;
	ShouldProduceTargetDataOnServer = true;
	
}



void AGATA_SingleLineTrace::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	AZeroLockCharacter* Hero = Cast<AZeroLockCharacter>(Ability->GetAvatarActorFromActorInfo());
	
	PrimaryPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	StartLocation.SourceAbility = OwningAbility;
	StartLocation.SourceActor = Hero;
	StartLocation.SourceComponent = Hero->GetMesh();
	StartLocation.LocationType =EGameplayAbilityTargetingLocationType::ActorTransform;
	//MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
}
