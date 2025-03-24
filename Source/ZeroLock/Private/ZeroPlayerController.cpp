// Fill out your copyright notice in the Description page of Project Settings.


#include "ZeroPlayerController.h"

#include "NetCodeBase/ClientPredictedActorBase.h"

uint32 AZeroPlayerController::RequestPredictedActorID()
{
	const uint32 NewID = NextPredictedActorID++;
	PredictedActors.Add(FZeroPredictedActorInfo( { NewID }) );
	return NewID;
}

void AZeroPlayerController::SetPredictedActor(uint32 ID, AClientPredictedActorBase* PredictedActor)
{
	if (auto pInfo = PredictedActors.FindByPredicate([ID](const FZeroPredictedActorInfo& Info)
	{
		return Info.ClientActorID == ID;
	}))
	{
		check (!pInfo->PredictedActor.IsValid());
		pInfo->PredictedActor = PredictedActor;

		// If both are valid, link up
		if (pInfo->PredictedActor.IsValid() && pInfo->ReplicatedActor.IsValid())
		{
			pInfo->ReplicatedActor->LinkReplicatedWithPredicted(pInfo->PredictedActor.Get());
		}
	}
}

void AZeroPlayerController::SetPredictedActorReplicatedActor(uint32 ID, AClientPredictedActorBase* ReplicatedActor)
{
	if (auto pInfo = PredictedActors.FindByPredicate([ID](const FZeroPredictedActorInfo& Info)
	{
		return Info.ClientActorID == ID;
	}))
	{
		check (!pInfo->ReplicatedActor.IsValid());
		pInfo->ReplicatedActor = ReplicatedActor;

		// If both are valid, link up
		if (pInfo->PredictedActor.IsValid() && pInfo->ReplicatedActor.IsValid())
		{
			pInfo->ReplicatedActor->LinkReplicatedWithPredicted(pInfo->PredictedActor.Get());
		}
	}
}
