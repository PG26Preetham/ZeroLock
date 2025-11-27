// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Objectives/ZeroObjective.h"

// Sets default values
AZeroObjective::AZeroObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectiveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Objective Mesh"));
	ObjectiveMesh->SetupAttachment(GetRootComponent());	
}

// Called when the game starts or when spawned
void AZeroObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZeroObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

