#include "EUW_AbilityPropertyEditor.h"
#include  "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "GAS/BaseGameplayAbility.h"
#include "EditorUtilityWidgetComponents.h"

void UEUW_AbilityPropertyEditor::PopulateAbilities()
{
	if (IsValid(AbilityScrollBox))
	{
		AbilityScrollBox->ClearChildren();
		for (TObjectIterator<UClass> classIterator; classIterator; ++classIterator)
		{
			UClass* Class = *classIterator;

			if (Class->GetName().Contains("SKEL"))
			{
				continue;
			}

			if (!Class->ClassGeneratedBy)
			{
				continue;
			}

			if (Class && Class->IsChildOf(UBaseGameplayAbility::StaticClass()) && Class != UBaseGameplayAbility::StaticClass())
			{
				UTextBlock* textBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
				textBlock->SetText(FText::FromString(Class->GetName()));
				
				UDetailsView* detailsView = WidgetTree->ConstructWidget<UDetailsView>(UDetailsView::StaticClass());
				detailsView->CategoriesToShow.Add("Icon");
				detailsView->SetObject(Class->GetDefaultObject());

				AbilityScrollBox->AddChild(textBlock);
				AbilityScrollBox->AddChild(detailsView);
			}
		}
	}
}
