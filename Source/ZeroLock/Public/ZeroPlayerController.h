// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroPlayerController.generated.h"
class AClientPredictedActorBase;

struct FZeroPredictedActorInfo
{
	/// The identifier. This is separate from the actor because we can create the ID first,
	/// then delay creating the client actor, so we need to know the *intended* ID
	uint32 ClientActorID = 0;

	/// The client predicted actor. Hopefully should be created before the server one replicates
	/// back to us, but in the case of a mis-prediction of lag, the server might send us the actor first
	TWeakObjectPtr<AClientPredictedActorBase> PredictedActor;

	/// The server replicated actor. 
	TWeakObjectPtr<AClientPredictedActorBase> ReplicatedActor;
};

/**
 * 
 */
UCLASS()
class ZEROLOCK_API AZeroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	uint32 RequestPredictedActorID();

	/// Local-only predicted actor ID (fine to wrap eventually)
	uint32 NextPredictedActorID = 0;

	/// Client predicted actors that are owned locally, waiting for the server copy to match up with
	TArray<FZeroPredictedActorInfo> PredictedActors;

	UFUNCTION()
	void SetPredictedActor(uint32 ID,AClientPredictedActorBase* PredictedActor);

	UFUNCTION()
	void SetPredictedActorReplicatedActor(uint32 ID,AClientPredictedActorBase* ReplicatedActor);
};
