// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorTestActor.h"

#include "GAS/BaseGameplayAbility.h"

// Sets default values
AEditorTestActor::AEditorTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEditorTestActor::BeginPlay()
{
	TArray<UClass*> foundClasses;

	for (TObjectIterator<UClass> classIterator; classIterator; ++classIterator)
	{
		UClass* Class = *classIterator;

		if (Class && Class->IsChildOf(UBaseGameplayAbility::StaticClass()) && Class != UBaseGameplayAbility::StaticClass())
		{
			foundClasses.Add(Class);
		}
	}
	
	for (UClass* Class : foundClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Class: %s"), *Class->GetName());
		// Iterate through properties using TFieldIterator
		for (TFieldIterator<FProperty> PropertyIt(Class); PropertyIt; ++PropertyIt)
		{
			FProperty* Property = *PropertyIt;
        
			FString PropertyName = Property->GetName();
			FString PropertyTypeName = Property->GetClass()->GetName();
        
			UE_LOG(LogTemp, Log, TEXT("Property: %s, Type: %s"), *PropertyName, *PropertyTypeName);
			
			// Get more detailed information
			//UE_LOG(LogTemp, Log, TEXT("  - CPP Type: %s"), *Property->GetCPPType());
			//UE_LOG(LogTemp, Log, TEXT("  - Size: %d bytes"), Property->GetSize());
			//UE_LOG(LogTemp, Log, TEXT("  - Offset: %d"), Property->GetOffset_ForInternal());
			//UE_LOG(LogTemp, Log, TEXT("  - Array Dim: %d"), Property->ArrayDim);
		}

	}
	
	
	Super::BeginPlay();
}

// Called every frame
void AEditorTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

