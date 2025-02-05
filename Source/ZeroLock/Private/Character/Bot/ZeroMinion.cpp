// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Bot/ZeroMinion.h"

// Sets default values
AZeroMinion::AZeroMinion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AZeroMinion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZeroMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZeroMinion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

