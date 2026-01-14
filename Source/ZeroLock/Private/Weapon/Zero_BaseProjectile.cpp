//Copyright Preetham Mukundan (C) 2026


#include "Weapon/Zero_BaseProjectile.h"

// Sets default values
AZero_BaseProjectile::AZero_BaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZero_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZero_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

