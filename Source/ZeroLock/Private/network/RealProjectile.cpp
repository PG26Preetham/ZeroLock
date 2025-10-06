// Fill out your copyright notice in the Description page of Project Settings.


#include "network/RealProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/BaseGameplayAbility.h"
#include "network/PredictedProjectile.h"


ARealProjectile::ARealProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	


	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	MovementComp->InitialSpeed = 3000.f;
	MovementComp->MaxSpeed = 3000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.f;
}

void ARealProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "ServerSpawn");
	if (OwningPredProj)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "Moving");
		FVector PredLoc = OwningPredProj->GetActorLocation();
		//FVector NewLoc = FMath::VInterpTo(GetActorLocation(), PredLoc, DeltaTime, CorrectionInterpSpeed);

		SetActorLocation(PredLoc);

		
		OwningPredProj->Destroy();
		OwningPredProj = nullptr;
		
	}
}

void ARealProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void ARealProjectile::InitVelocity(FVector Direction)
{
	if (MovementComp)
	{
		MovementComp->Velocity = Direction * MovementComp->InitialSpeed;
	}
}
