//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZL_CharacterSelector_Actor.generated.h"

class UZL_Character_Data_Asset;
class UZl_CharacterSelectionVM;

UCLASS()
class ZEROLOCK_API AZL_CharacterSelector_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZL_CharacterSelector_Actor();

protected:
	UFUNCTION()
	void OnMousePosChanged(FVector2D NewMousePos);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnViewModelFieldChanged(UZL_Character_Data_Asset* NewHero);


	// Components
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* CaptureComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	FRotator TargetSpringArmRot;

	// Function to change mesh + animation at runtime
	void SetPreview(USkeletalMesh* NewMesh, class UAnimSequence* NewAnim);


	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float RotationIntensity = 5.0f; // Max degrees to rotate

	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float InterpSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Control")
	float Maxangle = 10.0f;

private:
	// Stored reference to the VM so we can access the data
	UPROPERTY(Transient)
	TObjectPtr<UZl_CharacterSelectionVM> BoundVM;
};
