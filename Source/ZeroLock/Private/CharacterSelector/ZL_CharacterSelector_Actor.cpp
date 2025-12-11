// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelector/ZL_CharacterSelector_Actor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AZL_CharacterSelector_Actor::AZL_CharacterSelector_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	RootComponent = PreviewMesh;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	// Create capture component
	CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComp"));
	CaptureComp->SetupAttachment(SpringArm);



}

// Called when the game starts or when spawned
void AZL_CharacterSelector_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZL_CharacterSelector_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZL_CharacterSelector_Actor::SetPreview(USkeletalMesh* NewMesh, class UAnimSequence* NewAnim)
{
	if (NewMesh)
	{
		PreviewMesh->SetSkeletalMesh(NewMesh);
	}

	if (NewAnim)
	{
		PreviewMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		PreviewMesh->SetAnimation(NewAnim);
		PreviewMesh->Play(true); // loop
	}
}

