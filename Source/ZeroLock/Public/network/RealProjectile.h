//Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"

#include "RealProjectile.generated.h"

class APredictedProjectile;
class USphereComponent;
class UBaseGameplayAbility;

UCLASS()
class ZEROLOCK_API ARealProjectile : public AActor
{
	GENERATED_BODY()

public:
	ARealProjectile();

	void InitVelocity(FVector Direction);

	UPROPERTY()
	UBaseGameplayAbility* OwningAbility;

	UPROPERTY()
	APredictedProjectile* OwningPredProj;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly)
	float CorrectionInterpSpeed = 20.f; 
};
