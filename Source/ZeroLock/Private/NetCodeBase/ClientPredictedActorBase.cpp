// Fill out your copyright notice in the Description page of Project Settings.


#include "NetCodeBase/ClientPredictedActorBase.h"

#include "ZeroPlayerController.h"

// Sets default values
AClientPredictedActorBase::AClientPredictedActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

uint32 AClientPredictedActorBase::GenerateClientID(const UObject* WorldContext)
{
	if (auto PC = Cast<AZeroPlayerController>(GEngine->GetFirstLocalPlayerController(WorldContext->GetWorld())))
	{
		return PC->RequestPredictedActorID();
	}
	// Should never get here
	check(false)
	return 0;
}

void AClientPredictedActorBase::LinkReplicatedWithPredicted(AClientPredictedActorBase* PredictedActor)
{
	SetActorHiddenInGame(true);

	PredictedActor->SetActorEnableCollision(false);
	//PredictedActor->FollowReplicatedActor(this);
	//PredictedActor->HasAuthority()
	
}


void AClientPredictedActorBase::SetIdentifier(uint32 ClientIdref)
{
	ClientID = ClientIdref;
}

void AClientPredictedActorBase::SetIsPredictedCopy(bool IsPredicted)
{
	bIsPredicted = IsPredicted;
}

bool AClientPredictedActorBase::IsLocallyOwned() const
{
	if(GetWorld())
	{
		if(AZeroPlayerController* PC = Cast<AZeroPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld())))
		{
			return IsOwnedBy(PC);
		}
	}
	return false;
}

// Called when the game starts or when spawned
void AClientPredictedActorBase::BeginPlay()
{

	if(IsLocallyOwned() && GetWorld()->GetNetMode()==NM_Client)
	{
		if(AZeroPlayerController* PC = Cast<AZeroPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld())))
		{
			if(bIsPredicted)
			{
				
			}
			else
			{
				
			}
		}
	}
	Super::BeginPlay();

	
	
}

// Called every frame
void AClientPredictedActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

