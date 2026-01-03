// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ZL_Character_Data_Asset.generated.h"

class AZeroLockCharacter;

USTRUCT(BlueprintType)
struct FHeroTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// Pick your DataAsset in the DataTable field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UZL_Character_Data_Asset> HeroAsset;
};
/**
 * 
 */
UCLASS()
class ZEROLOCK_API UZL_Character_Data_Asset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AZeroLockCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DisplaySeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> DisplayAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor IconColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	
};
