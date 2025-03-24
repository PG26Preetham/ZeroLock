// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClientPredictedActorBase.generated.h"

UCLASS()
class ZEROLOCK_API AClientPredictedActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClientPredictedActorBase();

	UFUNCTION()
	uint32 GenerateClientID(const UObject* WorldContext);

	
	void LinkReplicatedWithPredicted(AClientPredictedActorBase* PredictedActor);

	

	uint32 ClientID;
	
	UFUNCTION()
	void SetIdentifier(uint32 ClientIdref);


	bool bIsPredicted;
	UFUNCTION()
	void SetIsPredictedCopy(bool IsPredicted);

	UFUNCTION()
	bool IsLocallyOwned() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
