// Copyright Preetham Mukundan (C) 2026

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Zero_BaseProjectile.h"
#include "ZL_Xayah_projectile.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZL_Xayah_projectile : public AZero_BaseProjectile
{
	GENERATED_BODY()
public:
	AZL_Xayah_projectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Change")
	class UMaterialInterface* HighlightMaterial;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "State")
	bool bIsReturning = false;

	UPROPERTY(Replicated,ReplicatedUsing = OnRep_IsActivatedToPull, BlueprintReadOnly, Category = "State")
	bool bIsActivatedToPull = false;
	
	UFUNCTION(BlueprintCallable, Category = "Feather")
	void StartReturn(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Feather")
	void SetAutoPull(bool bBool);

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
	void OnRep_IsActivatedToPull();
	
	void SendGameplayEventToACtor();


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> FeatherDownClassClass;
	
	void CheckISAutoActiavted();
	virtual void HitEventCallBack(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OverlapEventCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
