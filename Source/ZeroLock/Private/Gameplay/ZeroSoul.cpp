// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ZeroSoul.h"

// Sets default values
AZeroSoul::AZeroSoul()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Core = CreateDefaultSubobject<USceneComponent>("Core");
	SetRootComponent(Core);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Soul Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));
	Mesh->SetRelativeScale3D(FVector(0.2f));

	LifeTime = 2.0f;
	InitialLifeSpan = LifeTime;

}

// Called when the game starts or when spawned
void AZeroSoul::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &AZeroSoul::OnHit);
	
}

void AZeroSoul::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{

	//UE_LOG(LogTemp, Warning, TEXT("Pepelaugh"));
	
}


// Called every frame
void AZeroSoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

