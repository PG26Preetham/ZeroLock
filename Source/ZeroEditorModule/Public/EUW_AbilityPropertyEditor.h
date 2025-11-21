#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/DetailsView.h"
#include "EUW_AbilityPropertyEditor.generated.h"

class UEditorUtilityScrollBox;

UCLASS(BlueprintType, Blueprintable)
class ZEROEDITORMODULE_API UEUW_AbilityPropertyEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

	
	UFUNCTION(BlueprintCallable, Category = Editor)
	void PopulateAbilities();
	
	UFUNCTION(BlueprintCallable)
	UObject* GetClassDefault(UClass* inClass) { return inClass->GetDefaultObject(); }

	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TArray<FName> AbilityCategories = { "Icon" };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Editor, meta = (BindWidget), meta = (AllowPrivateAccess))
	TObjectPtr<UEditorUtilityScrollBox> AbilityScrollBox;

};
