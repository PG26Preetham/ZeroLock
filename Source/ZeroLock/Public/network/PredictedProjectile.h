//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredictedProjectile.generated.h"

class USphereComponent;

UCLASS()
class ZEROLOCK_API APredictedProjectile : public AActor
{
	GENERATED_BODY()

public:
	APredictedProjectile();

	void InitVelocity(FVector Direction);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComp;

};
