// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZL_CharacterSelector_Actor.generated.h"

UCLASS()
class ZEROLOCK_API AZL_CharacterSelector_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZL_CharacterSelector_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Components
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* CaptureComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	// Function to change mesh + animation at runtime
	void SetPreview(USkeletalMesh* NewMesh, class UAnimSequence* NewAnim);
};
